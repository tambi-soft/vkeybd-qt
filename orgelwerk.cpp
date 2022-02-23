#include "orgelwerk.h"

Orgelwerk::Orgelwerk(QString label, QWidget *parent) : QWidget(parent)
{
    this->label = label;
    
    //this->interface_audio = new InterfaceAlsa(label);
    //this->interface_audio = new InterfaceJack(label);
    this->list_of_audio_interfaces.append(new InterfaceAlsa("alsa-midi-"+label));
    
    drawGUI();
    initInputThread();
    
    installEventFilter(this);
}

void Orgelwerk::drawGUI()
{
    //this->channels = new MIDIChannelSelector(this->interface_audio);
    this->channels = new MIDIChannelSelector(this->list_of_audio_interfaces);
    connect(this->channels, &MIDIChannelSelector::newAudioInterfaceRequested, this, &Orgelwerk::addNewAudioInterface);
    this->keys = new MIDIKeySelector;
    this->pitch = new MIDIPitchWheel;
    
    connect(this->channels, &MIDIChannelSelector::eventFiltered, this, &Orgelwerk::eventFilter);
    
    connect(this->pitch, &MIDIPitchWheel::pitchWheelMoved, this, &Orgelwerk::pitchWheelMoved);
    
    this->grid = new QGridLayout;
    setLayout(this->grid);
    
    QGroupBox *group_keys = new QGroupBox("Keys");
    QGroupBox *group_pitch = new QGroupBox("Pitch");
    
    QVBoxLayout *layout_keys = new QVBoxLayout;
    QHBoxLayout *layout_pitch = new QHBoxLayout;
    
    group_keys->setLayout(layout_keys);
    group_pitch->setLayout(layout_pitch);
    
    layout_keys->addWidget(this->keys);
    layout_pitch->addWidget(this->pitch);
    
    this->button_panic->setText("Panic! [Esc]");
    connect(this->button_panic, &QPushButton::clicked, this, &Orgelwerk::panicKeyPressed);
    QString stylesheet_panic = "QPushButton {"
                         "  color: white;"
                         "  background-color: darkred;"
                         "}"
                         "QPushButton:pressed {"
                         "  color: white;"
                         "  background-color: yellow;"
                         "}";
    this->button_panic->setStyleSheet(stylesheet_panic);
    
    this->button_stop_all->setText("Stop All [Del]");
    connect(this->button_stop_all, &QPushButton::clicked, this, &Orgelwerk::stopAllPressed);
    QString stylesheet_stop = "QPushButton {"
                         "  color: white;"
                         "  background-color: darkorange;"
                         "}"
                         "QPushButton:pressed {"
                         "  color: white;"
                         "  background-color: yellow;"
                         "}";
    this->button_stop_all->setStyleSheet(stylesheet_stop);
    QLayout *layout_panic_stop = new QHBoxLayout;
    layout_panic_stop->addWidget(this->button_panic);
    layout_panic_stop->addWidget(this->button_stop_all);
    
    QLabel *label_key_shift_master = new QLabel("Key Shift");
    this->key_shift_master = new MIDIKeyShiftWidget;
    
    this->volume = new MIDIMastervolume;
    connect(volume, &MIDIMastervolume::sliderMoved, this, &Orgelwerk::volumeSliderMoved);
    
    //showChannelsReal(0);
    //showChannelsImage(1);
    showChannelsSummary(1);
    this->grid->addWidget(label_key_shift_master, 2, 0);
    this->grid->addWidget(this->key_shift_master, 3, 0);
    //this->grid->addWidget(key_shift_widget, 2, 0);
    this->grid->addWidget(volume, 2, 1, 2, 1);
    //this->grid->addWidget(label_volume_master, 2, 1);
    //this->grid->addWidget(this->slider_volume_master, 3, 1);
    this->grid->addWidget(group_keys, 4, 0, 1, 2);
    this->grid->addWidget(group_pitch, 5, 0, 1, 2);
    drawNotesKeyboard(6);
    //drawPianoKeyboard(7);
    drawPCKeyboard(8);
    this->grid->addLayout(layout_panic_stop, 10, 0, 1, 2);
    
    //this->grid->setSizeConstraint( QLayout::SetFixedSize );
}

void Orgelwerk::drawNotesKeyboard(int grid_row)
{
    this->notes = new KeyboardNotes;
    
    this->grid->addWidget(this->notes, grid_row, 0, 1, 2);
}
void Orgelwerk::drawPianoKeyboard(int grid_row)
{
    this->piano = new KeyboardPiano;
    
    this->grid->addWidget(this->piano, grid_row, 0, 1, 2);
}
void Orgelwerk::drawPCKeyboard(int grid_row)
{
    this->pc = new KeyboardPC;
    connect(this->pc, &KeyboardPC::MIDIPress, this, &Orgelwerk::keyMIDIDown);
    connect(this->pc, &KeyboardPC::MIDIRelease, this, &Orgelwerk::keyMIDIUp);
    
    this->grid->addWidget(this->pc, grid_row, 0, 1, 2);
}

void Orgelwerk::showChannelsReal(int grid_row)
{
    QGroupBox *group_channels = new QGroupBox("Channels");
    QVBoxLayout *layout_channels = new QVBoxLayout;
    group_channels->setLayout(layout_channels);
    layout_channels->setContentsMargins(0, 0, 0, 0);
    
    this->scroll_channels = new QScrollArea;
    scroll_channels->setWidget(this->channels);
    
    QPushButton *button_channels_dialog = new QPushButton("Show in own Window");
    connect(button_channels_dialog, &QPushButton::clicked, this, [this]{ this->showChannelDetails(false); });
    
    layout_channels->addWidget(scroll_channels);
    layout_channels->addWidget(button_channels_dialog);
    
    this->grid->addWidget(group_channels, grid_row, 0, 1, 2);
}
void Orgelwerk::showChannelsImage(int grid_row)
{
    this->button_channels = new QPushButton;
    connect(this->button_channels, &QPushButton::clicked, this, [this]{ this->showChannelDetails(true); });
    
    QGroupBox *group_channels_pixmap = new QGroupBox("Channels");
    QVBoxLayout *layout_channels_pixmap = new QVBoxLayout;
    layout_channels_pixmap->setContentsMargins(0, 0, 0, 0);
    group_channels_pixmap->setLayout(layout_channels_pixmap);
    layout_channels_pixmap->addWidget(this->button_channels);
    
    this->grid->addWidget(group_channels_pixmap, grid_row, 0, 1, 2);
    
    updateChannelsSchreenshot();
}
void Orgelwerk::showChannelsSummary(int grid_row)
{
    QGroupBox *group_channels = new QGroupBox("Channels");
    QGridLayout *layout_channels = new QGridLayout;
    group_channels->setLayout(layout_channels);
    
    this->midi_channels_summary = new MIDIChannelsSummary;
    
    this->button_channels_dialog->setText("Edit MIDI Channels");
    connect(button_channels_dialog, &QPushButton::clicked, this, [this]{ this->showChannelDetails(false); });
    
    this->button_resend_midi->setText("Resend MIDI Settings [Ins]");
    connect(button_resend_midi, &QPushButton::clicked, this, &Orgelwerk::resendMIDIControls);
    
    //layout_channels->addWidget(scroll, 0, 0, 1, 2);
    layout_channels->addWidget(this->midi_channels_summary, 0, 0, 1, 2);
    layout_channels->addWidget(button_channels_dialog, 1, 0);
    layout_channels->addWidget(button_resend_midi, 1, 1);
    
    this->grid->addWidget(group_channels, grid_row, 0, 1, 2);
    
    channelsSummaryUpdate();
}
void Orgelwerk::channelsSummaryUpdate()
{
    QList<QMap<QString,QVariant>> data = this->channels->listOfChannels(true);
    
    this->midi_channels_summary->showData(data);
}

void Orgelwerk::initInputThread()
{
    this->thread_input = new QThread(this);
    this->worker_input = new InputKeyboardRawThread();
    this->worker_input->moveToThread(this->thread_input);
    
    this->thread_input->start();
    
    //this->worker_input->start();
}

Orgelwerk::~Orgelwerk()
{
    this->thread_input->exit();
}

void Orgelwerk::addNewAudioInterface(QString label)
{
    this->list_of_audio_interfaces.append(new InterfaceAlsa(label));
    
    this->channels->setListOfAudioOutputs(this->list_of_audio_interfaces);
}

void Orgelwerk::keyDown(int keycode)
{
    
    this->number_of_keys_down++;
    
    qDebug() << "keyDown: "+this->label+" "+ QString::number(keycode);
    this->pc->keyDown(keycode);
}
void Orgelwerk::keyUp(int keycode)
{
    if (this->number_of_keys_down >= 1)
    {
        this->number_of_keys_down--;
    }
    
    qDebug() << "keyUp:   "+this->label+" "+ QString::number(keycode);
    this->pc->keyUp(keycode);
}

void Orgelwerk::panicKeyPressed()
{
    if (this->piano)
    {
        this->piano->allKeysUp();
    }
    
    if (this->notes)
    {
        this->notes->allKeysUp();
    }
    
    this->pc->allKeysUp();
    
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(true);
    for (int c=0; c < list_of_channels.length(); c++)
    {
        //this->interface_audio->keyPanicEvent(list_of_channels.at(c)["channel"].toInt());
        int channel = list_of_channels.at(c)["channel"].toInt();
        this->list_of_audio_interfaces.at(list_of_channels.at(c)["interface_index"].toInt())->keyPanicEvent(channel);
    }
    
    this->number_of_keys_down = 0;
}
void Orgelwerk::stopAllPressed()
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(true);
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int channel = list_of_channels.at(c)["channel"].toInt();
        this->list_of_audio_interfaces.at(list_of_channels.at(c)["interface_index"].toInt())->keyStopAllEvent(channel);
    }
}

void Orgelwerk::keyMIDIHelper(int midicode, QString mode)
{
    qDebug() << "################ " << this->number_of_keys_down;
    
    // -12: The lower full octave on the keyboard is in the midi-range of 12 - 23.
    // For being able to add some even deeper notes to the left.
    // Therefore we get an offset of 12 we have to compensate here.
    int keycode = midicode - 12;
    
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(true);
    
    int master_key_shift = this->key_shift_master->value();
    QList<int> list_of_keys = this->keys->getListOfSelectedKeys();
    for (int k=0; k < list_of_keys.length(); k++)
    {
        for (int c=0; c < list_of_channels.length(); c++)
        {
            int channel = list_of_channels.at(c)["channel"].toInt();
            int interface_index = list_of_channels.at(c)["interface_index"].toInt();
            int key_shift = list_of_channels.at(c)["key_shift"].toInt();
            int key_min = list_of_channels.at(c)["key_min"].toInt();
            int key_max = list_of_channels.at(c)["key_max"].toInt(); 
            
            int m_code = keycode + list_of_keys.at(k);
            int m_code_shifted = m_code + key_shift + master_key_shift;
            
            if ((key_min <= m_code && key_max >= m_code) || mode == "pitch")
            {
                if (mode == "down")
                {
                    this->list_of_audio_interfaces.at(interface_index)->keyPressEvent(channel, m_code_shifted);
                    
                    if (this->notes) 
                    {
                        this->notes->keyPressed(m_code_shifted);
                    }
                }
                else if (mode == "up")
                {
                    this->list_of_audio_interfaces.at(interface_index)->keyReleaseEvent(channel, m_code_shifted);
                    
                    if (this->notes)
                    {
                        this->notes->keyReleased(m_code_shifted);
                    }
                }
                else if (mode == "pitch")
                {
                    this->list_of_audio_interfaces.at(interface_index)->keyPitchbendEvent(channel, midicode);
                }
            }
        }
    }
    
    if (mode == "down")
    {
        if (this->piano)
        {
            this->piano->keyPressed(midicode);
        }
    }
    else if (mode == "up")
    {
        if (this->piano)
        {
            this->piano->keyReleased(midicode);
        }
    }
}
void Orgelwerk::keyMIDIDown(int midicode)
{
    keyMIDIHelper(midicode, "down");
}
void Orgelwerk::keyMIDIUp(int midicode)
{
    keyMIDIHelper(midicode, "up");
}

// to move pitch wheel by keyboard
void Orgelwerk::movePitchWheel(int key)
{
    this->pitch->movePitchWheel(key);
}
void Orgelwerk::pitchWheelMoved(int pitch)
{
    keyMIDIHelper(pitch, "pitch");
}

void Orgelwerk::keySustain(bool pressed)
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels();
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int interface_index = list_of_channels.at(c)["interface_index"].toInt();
        int channel = list_of_channels.at(c)["channel"].toInt();
        this->list_of_audio_interfaces.at(interface_index)->keySustainEvent(channel, pressed);
    }
}

void Orgelwerk::keySostenuto(bool pressed)
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels();
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int interface_index = list_of_channels.at(c)["interface_index"].toInt();
        int channel = list_of_channels.at(c)["channel"].toInt();
        this->list_of_audio_interfaces.at(interface_index)->keySostenutoEvent(channel, pressed);
    }
}

void Orgelwerk::keySoft(bool pressed)
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(true);
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int interface_index = list_of_channels.at(c)["interface_index"].toInt();
        int channel = list_of_channels.at(c)["channel"].toInt();
        int volume = list_of_channels.at(c)["volume"].toInt();
        this->list_of_audio_interfaces.at(interface_index)->keySoftEvent(channel, pressed, volume);
    }
}

void Orgelwerk::volumeSliderMoved(int value)
{
    this->channels->volumeDCAChanged(value);
}

void Orgelwerk::showChannelDetails(bool update_preview)
{
    QDialog *dialog = new QDialog();
    connect(dialog, &QDialog::rejected, this, &Orgelwerk::channelsDialogRejected);
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    
    //QWidget *channel_details = this->scroll_channels->takeWidget();
    //layout_dialog->addWidget(channel_details);
    layout_dialog->addWidget(this->channels);
    //layout_dialog->addWidget(this->scroll_channels);
    //layout_dialog->setGeometry(this->scroll_channels->geometry());
    
    dialog->layout()->setSizeConstraint( QLayout::SetFixedSize );
    
    dialog->exec();
    
    if (update_preview)
    {
        updateChannelsSchreenshot();
    }
    
    //if (this->scroll_channels != nullptr)
    //{
    qDebug() << "||==||";
    //3qDebug() << this->scroll_channels;
    
    //layout_dialog->deleteLater();
    //dialog->deleteLater();
    
        //this->scroll_channels->setWidget(this->channels);
    //}
    //this->channels->show();
}

void Orgelwerk::channelsDialogRejected()
{
    qDebug() << "rejected";
    channelsSummaryUpdate();
    
    /*
    qDebug() << this->channels;
    this->scroll_channels->setWidget(this->channels);
    //this->scroll_channels->widget()->show();
    this->scroll_channels->show();
    this->channels->show();
    */
}

void Orgelwerk::updateChannelsSchreenshot()
{
    QPixmap channels_pixmap = this->channels->grab();
    
    float w = channels_pixmap.width();
    float h = channels_pixmap.height();
    
    int scale_factor = 380;
    float scale = scale_factor / w;
    w = scale_factor;
    h = h * scale;
    
    channels_pixmap = channels_pixmap.scaled(w, h, Qt::KeepAspectRatio);
    
    //channels_pixmap = channels_pixmap.scaled(this->button_channels->size(), Qt::KeepAspectRatio);
    
    QIcon ButtonIcon(channels_pixmap);
    this->button_channels->setIcon(ButtonIcon);
    this->button_channels->setIconSize(channels_pixmap.rect().size());
}

void Orgelwerk::resendMIDIControls()
{
    this->channels->resendMIDIControls();
}

QList<QMap<QString,QVariant>> Orgelwerk::listOfChannels(bool only_activated)
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(only_activated);
    return list_of_channels;
}

void Orgelwerk::setListOfChannels(QList<QMap<QString,QVariant>> data)
{
    this->channels->setListOfChannels(data);
}

bool Orgelwerk::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyRelease)
    {
        emit eventFiltered(obj, ev);
        
        return true;
    }
    
    return false;
}

bool Orgelwerk::areKeysPressed()
{
    if (this->number_of_keys_down > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
