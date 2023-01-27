#include "orgelwerk.h"

Orgelwerk::Orgelwerk(int keyboard_id, int tab_id, InterfaceAudio *interface_audio, QString label, QWidget *parent) : QWidget(parent)
{
    this->keyboard_id = keyboard_id;
    this->tab_id = tab_id;
    this->interface_audio = interface_audio;
    this->label = label;
    
    this->interface_audio->createNewPort(QString::number(keyboard_id+1)+"-"+label);
    
    drawGUI();
    
    installEventFilter(this);
}

void Orgelwerk::drawGUI()
{
    this->channels = new MIDIChannelSelector(this->interface_audio, this->tab_id);
    connect(this->channels, &MIDIChannelSelector::newAudioInterfaceRequested, this, &Orgelwerk::addNewAudioInterface);
    this->keys = new MIDIKeySelector;
    this->pitch = new MIDIPitchWheel;
    this->volume = new MIDIMasterVolume;
    this->key_shift_master = new MIDIKeyShiftWidget;
    
    connect(this->channels, &MIDIChannelSelector::eventFiltered, this, &Orgelwerk::eventFilter);
    connect(this->channels, &MIDIChannelSelector::closed, this, &Orgelwerk::channelsSummaryUpdate);
    connect(this->channels, &MIDIChannelSelector::destroyed, this, &Orgelwerk::channelsSummaryUpdate);
    
    connect(this->pitch, &MIDIPitchWheel::pitchWheelMoved, this, &Orgelwerk::pitchWheelMoved);
    
    connect(volume, &MIDIMasterVolume::sliderMoved, this, &Orgelwerk::volumeSliderMoved);
    
    this->scroll_channels = new QScrollArea;
    this->scroll_channels->setWidget(this->channels);
    this->scroll_channels->setWidgetResizable(false);
    this->scroll_channels->setWindowTitle("Keyboard " + QString::number(this->keyboard_id) + "-" + this->label);
    connect(this->scroll_channels, &QScrollArea::destroyed, this, &Orgelwerk::channelsSummaryUpdate);
    
    this->grid = new QGridLayout;
    setLayout(this->grid);
    
    this->group_keys = new QGroupBox("Keys");
    this->group_pitch = new QGroupBox("Pitch && Volume");
    
    QVBoxLayout *layout_keys = new QVBoxLayout;
    QGridLayout *layout_pitch = new QGridLayout;
    
    this->group_keys->setLayout(layout_keys);
    this->group_pitch->setLayout(layout_pitch);
    
    QLabel *label_key_shift_master = new QLabel("Key Shift");
    
    layout_keys->addWidget(this->keys);
    QGridLayout *layout_pitch_volume = new QGridLayout;
    layout_pitch_volume->addWidget(label_key_shift_master, 0, 0);
    layout_pitch_volume->addWidget(this->key_shift_master, 1, 0);
    layout_pitch_volume->addWidget(this->volume->label_volume, 0, 1);
    layout_pitch_volume->addWidget(this->volume->slider_volume, 1, 1);
    
    layout_pitch->addLayout(layout_pitch_volume, 0, 0, 1, 2);
    layout_pitch->addWidget(this->pitch->label_tether, 1, 0);
    layout_pitch->addWidget(this->pitch->slider_tether, 2, 0);    
    
    layout_pitch->addWidget(this->volume->label_tether, 1, 1);
    layout_pitch->addWidget(this->volume->slider_tether, 2, 1);
    
    layout_pitch->addWidget(this->pitch->label_pitch, 3, 0);
    layout_pitch->addWidget(this->pitch->slider_pitch, 4, 0, 1, 2);
    
    this->button_panic->setText("Panic! [Esc]");
    connect(this->button_panic, &QPushButton::clicked, this, &Orgelwerk::panicKeyPressed);
    this->button_panic->setObjectName("button_panic");
    
    this->button_stop_all->setText("Stop All [Del]");
    connect(this->button_stop_all, &QPushButton::clicked, this, &Orgelwerk::stopAllPressed);
    this->button_stop_all->setObjectName("button_stop");
    
    QLayout *layout_panic_stop = new QHBoxLayout;
    layout_panic_stop->addWidget(this->button_panic);
    layout_panic_stop->addWidget(this->button_stop_all);
    
    showChannelsSummary(1);
    this->grid->addWidget(this->group_keys, 4, 0, 1, 2);
    this->grid->addWidget(this->group_pitch, 5, 0, 1, 2);
    drawNotesKeyboard(6);
    drawPianoKeyboard(7);
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
    this->piano->hide();
}
void Orgelwerk::drawPCKeyboard(int grid_row)
{
    this->pc = new KeyboardPC;
    connect(this->pc, &KeyboardPC::MIDIPress, this, &Orgelwerk::keyMIDIDown);
    connect(this->pc, &KeyboardPC::MIDIRelease, this, &Orgelwerk::keyMIDIUp);
    connect(this->pc, &KeyboardPC::octaveDown, this, &Orgelwerk::octaveDown);
    connect(this->pc, &KeyboardPC::octaveUp, this, &Orgelwerk::octaveUp);
    
    this->grid->addWidget(this->pc, grid_row, 0, 1, 2);
}

void Orgelwerk::showChannelsSummary(int grid_row)
{
    QGroupBox *group_channels = new QGroupBox("Channels");
    QGridLayout *layout_channels = new QGridLayout;
    group_channels->setLayout(layout_channels);
    
    this->midi_channels_summary = new MIDIChannelsSummary;
    
    this->button_channels_dialog->setText("Edit MIDI Channels");
    connect(button_channels_dialog, &QPushButton::clicked, this, &Orgelwerk::showChannelDetails);
    
    this->button_resend_midi->setText("Resend MIDI Settings [Ins]");
    connect(button_resend_midi, &QPushButton::clicked, this, &Orgelwerk::resendMIDIControls);
    
    this->check_resend_midi_auto->setToolTip("Automatically resend MIDI settings if tab activated");
    
    layout_channels->addWidget(this->midi_channels_summary, 0, 0, 1, 3);
    layout_channels->addWidget(button_channels_dialog, 1, 0);
    layout_channels->addWidget(button_resend_midi, 1, 1);
    layout_channels->addWidget(this->check_resend_midi_auto, 1, 2);
    
    this->grid->addWidget(group_channels, grid_row, 0, 1, 2);
    
    channelsSummaryUpdate();
}
void Orgelwerk::channelsSummaryUpdate()
{
    QList<QMap<QString,QVariant>> data = this->channels->listOfChannels(true);
    
    this->midi_channels_summary->showData(data);
    this->scroll_channels->hide();
}

Orgelwerk::~Orgelwerk()
{
    //this->thread_input->quit();
    //this->thread_input->exit();
}

void Orgelwerk::addNewAudioInterface(QString label)
{
    /*
    if (this->audio_system == OutputSystem::Alsa)
    {
        this->list_of_audio_interfaces.append(new InterfaceAlsa(label));
    }
    else if (this->audio_system == OutputSystem::Jack)
    {
        this->list_of_audio_interfaces.append(new InterfaceJack(label));
    }
    
    this->channels->setListOfAudioOutputs(this->list_of_audio_interfaces);
    */
}

void Orgelwerk::keyDownRaw(int keycode)
{
    this->pc->keyDownRaw(keycode);
}
void Orgelwerk::keyUpRaw(int keycode)
{
    this->pc->keyUpRaw(keycode);
}

void Orgelwerk::panicKeyPressed()
{
    this->map_of_keys_down.clear();
    
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
        this->interface_audio->keyPanicEvent(this->tab_id, list_of_channels.at(c)["channel"].toInt());
        //int channel = list_of_channels.at(c)["channel"].toInt();
        //this->list_of_audio_interfaces.at(list_of_channels.at(c)["interface_index"].toInt())->keyPanicEvent(channel);
    }
}
void Orgelwerk::stopAllPressed()
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(true);
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int channel = list_of_channels.at(c)["channel"].toInt();
        //this->list_of_audio_interfaces.at(list_of_channels.at(c)["interface_index"].toInt())->keyStopAllEvent(channel);
    }
}

void Orgelwerk::keyMIDIHelper(int midicode, MIDIMode mode)
{
    // -12: The lower full octave on the keyboard is in the midi-range of 12 - 23.
    // For being able to add some even deeper notes to the left.
    // Therefore we get an offset of 12 we have to compensate here.
    int keycode = midicode - 12;
    
    if (mode == MIDIMode::Press)
        keyShiftMapAdd(keycode);
    
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
            int tremolo = list_of_channels.at(c)["tremolo"].toInt();
            int velocity = list_of_channels.at(c)["velocity"].toInt();
            qDebug() << "velocity: " << velocity;
            
            int m_code = keycode + list_of_keys.at(k);
            
            if ((key_min <= m_code && key_max >= m_code) || mode == MIDIMode::PitchBend)
            {
                if (mode == MIDIMode::Press)
                {
                    int m_code_shifted = m_code + key_shift + master_key_shift;
                    
                    if (tremolo == 0)
                    {
                        //this->list_of_audio_interfaces.at(interface_index)->keyPressEvent(channel, m_code_shifted);
                        this->interface_audio->keyPressEvent(this->tab_id, channel, m_code_shifted, velocity);
                    }
                    else
                    {
                        tremoloThreadStart(interface_index, channel, m_code_shifted, tremolo);
                    }
                    
                    if (this->notes)
                        this->notes->keyPressed(m_code_shifted);
                }
                else if (mode == MIDIMode::Release)
                {
                    //QList<int> keys = this->map_of_keys_down.keys();
                    //for (int i=0; i < keys.length(); i++)
                    //{
                    //    keyShiftMapRemove(keys.at(i), keycode);
                        
                        //int m_code_shifted = m_code + key_shift + keys.at(i);
                        int m_code_shifted = m_code + key_shift + master_key_shift;
                        
                        this->interface_audio->keyReleaseEvent(this->tab_id, channel, m_code_shifted, velocity);
                        
                        tremoloThreadStop(channel, m_code_shifted);
                        
                        if (this->notes)
                            this->notes->keyReleased(m_code_shifted);
                    //}
                }
                else if (mode == MIDIMode::PitchBend)
                {
                    this->interface_audio->keyPitchbendEvent(this->tab_id, channel, midicode);
                }
            }
        }
    }
    
    if (mode == MIDIMode::Press)
    {
        if (this->piano)
            this->piano->keyPressed(midicode);
    }
    else if (mode == MIDIMode::Release)
    {
        if (this->piano)
            this->piano->keyReleased(midicode);
    }
}
void Orgelwerk::keyMIDIDown(int midicode)
{
    keyMIDIHelper(midicode, MIDIMode::Press);
}
void Orgelwerk::keyMIDIUp(int midicode)
{
    keyMIDIHelper(midicode, MIDIMode::Release);
}
void Orgelwerk::tremoloThreadStart(int interface_index, int channel, int m_code_shifted, int tremolo)
{
    QString worker_code = QString::number(channel)+"_"+QString::number(m_code_shifted);
    if (!this->map_of_tremolo_threads.contains(worker_code))
    {
        TremoloWorker *worker = new TremoloWorker(
                    interface_index,
                    tremolo,
                    channel,
                    m_code_shifted
                    );
        connect(worker, &TremoloWorker::notePlay, this, &Orgelwerk::notePlay);
        connect(worker, &TremoloWorker::noteStop, this, &Orgelwerk::noteStop);
        
        QThread *thread = new QThread(this);
        worker->moveToThread(thread);
        thread->start();
        
        this->map_of_tremolo_threads[worker_code] = thread;
    }
}
void Orgelwerk::tremoloThreadStop(int channel, int m_code_shifted)
{
    QString worker_code = QString::number(channel)+"_"+QString::number(m_code_shifted);
    if (this->map_of_tremolo_threads.contains(worker_code))
    {
        this->map_of_tremolo_threads[worker_code]->quit();
        this->map_of_tremolo_threads[worker_code]->deleteLater();
        this->map_of_tremolo_threads.remove(worker_code);
    }
}
void Orgelwerk::keyShiftMapAdd(int keycode)
{
    int keyshift = this->key_shift_master->value();
    if (!this->map_of_keys_down[keyshift].contains(keycode))
    {
        this->map_of_keys_down[keyshift].append(keycode);
    }
    qDebug() << "DOWN: " << this->map_of_keys_down;
}
void Orgelwerk::keyShiftMapRemove(int key, int keycode)
{
    int pos = this->map_of_keys_down[key].indexOf(keycode);
    if (pos > -1)
        this->map_of_keys_down[key].removeAt(pos);
    qDebug() << "UP: " << this->map_of_keys_down;
}

void Orgelwerk::notePlay(int interface_index, int channel, int note)
{
    //this->list_of_audio_interfaces.at(interface_index)->keyPressEvent(channel, note);
}
void Orgelwerk::noteStop(int interface_index, int channel, int note)
{
    //this->list_of_audio_interfaces.at(interface_index)->keyReleaseEvent(channel, note);
}

// to move pitch wheel by keyboard
void Orgelwerk::movePitchWheel(int key)
{
    this->pitch->movePitchWheel(key);
}
void Orgelwerk::pitchWheelMoved(int pitch)
{
    keyMIDIHelper(pitch, MIDIMode::PitchBend);
}

void Orgelwerk::keySustain(bool pressed)
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels();
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int interface_index = list_of_channels.at(c)["interface_index"].toInt();
        int channel = list_of_channels.at(c)["channel"].toInt();
        this->interface_audio->keySustainEvent(this->tab_id, channel, pressed);
    }
}

void Orgelwerk::keySostenuto(bool pressed)
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels();
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int interface_index = list_of_channels.at(c)["interface_index"].toInt();
        int channel = list_of_channels.at(c)["channel"].toInt();
        this->interface_audio->keySostenutoEvent(this->tab_id, channel, pressed);
    }
}

void Orgelwerk::keySoft(bool pressed)
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(true);
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int interface_index = list_of_channels.at(c)["interface_index"].toInt();
        int channel = list_of_channels.at(c)["channel"].toInt();
        this->interface_audio->keySoftEvent(this->tab_id, channel, pressed);
    }
}

void Orgelwerk::volumeSliderMoved(int value)
{
    this->channels->volumeDCAChanged(value);
}

void Orgelwerk::showChannelDetails()
{
    QDesktopWidget *desktop = new QDesktopWidget;
    int screen_number = desktop->screenNumber(this);
    QSize screen_size = QGuiApplication::screens().at(screen_number)->size();
    
    QMargins margins = this->scroll_channels->contentsMargins();
    
    if (this->channels->size().width() > screen_size.width())
    {
        int scroll_bar_height = this->scroll_channels->horizontalScrollBar()->sizeHint().height();
        this->scroll_channels->resize(
                    screen_size.width(),
                    this->channels->height() + margins.top() + margins.bottom() + scroll_bar_height
                    );
    }
    else
    {
        this->scroll_channels->resize(
                    this->channels->width() + margins.left() + margins.right(),
                    this->channels->height() + margins.top() + margins.bottom()
                    );
    }
    
    this->scroll_channels->show();
}

void Orgelwerk::channelsDialogRejected()
{
    qDebug() << "REJECTED";
    channelsSummaryUpdate();
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
QMap<QString,QVariant> Orgelwerk::getParams()
{
    QMap<QString,QVariant> result;
    
    result["key_shift"] = this->key_shift_master->value();
    result["volume"] = this->volume->getValue();
    result["keys"] = this->keys->getBitmaskOfKeys();
    
    QMap<QString,int> pitchwheel = this->pitch->getData();
    
    //for (int i=0; i < pitchwheel.keys().length(); i++)
    for (int i=0; i < pitchwheel.count(); i++)
    {
        //result[pitchwheel.keys().at(i)] = pitchwheel[pitchwheel.keys().at(i)];
        result[pitchwheel.keys().at(i)] = pitchwheel[pitchwheel.keys().at(i)];
    }
    
    return result;
}
void Orgelwerk::restoreParams(QMap<QString,QVariant> data)
{
    this->channels->restoreParams(data);
    channelsSummaryUpdate();
    
    QMap<QString,QVariant> main = data["main"].toMap();
    this->key_shift_master->setValue(main["key_shift"].toInt());
    this->volume->setValue(main["volume"].toInt());
    this->keys->restoreBitmaskOfKeys(main["keys"].toString());
    this->pitch->setData(main);
    this->check_resend_midi_auto->setChecked(main["resend_midi_auto"].toBool());
}

void Orgelwerk::showHideGUIElements(GUIElements elements, bool show)
{
    if (elements == GUIElements::MIDIKeys)
    {
        if (show)
            this->group_keys->show();
        else
            this->group_keys->hide();
    }
    else if (elements == GUIElements::PitchAndVolume)
    {
        if (show)
            this->group_pitch->show();
        else
            this->group_pitch->hide();
    }
    else if (elements == GUIElements::Notes)
    {
        if (show)
            this->notes->show();
        else
            this->notes->hide();
    }
    else if (elements == GUIElements::KeyboardPC)
    {
        if (show)
            this->pc->show();
        else
            this->pc->hide();
    }
    else if (elements == GUIElements::KeyboardPiano)
    {
        if (show)
            this->piano->show();
        else
            this->piano->hide();
    }
}

void Orgelwerk::octaveDown()
{
    this->key_shift_master->lowerShiftKeyPressed();
}
void Orgelwerk::octaveUp()
{
    this->key_shift_master->higherShiftKeyPressed();
}
void Orgelwerk::globalPitchShiftChanged(int value, bool is_relative)
{
    if (! is_relative)
    {
        this->key_shift_master->setValue(value);
    }
    else
    {
        int value_current = this->key_shift_master->value();
        qDebug() << "old: " << value << " new: " << value;
        value_current += value;
        this->key_shift_master->setValue(value_current);
    }
}

void Orgelwerk::showEvent(QShowEvent *event)
{
    if (this->check_resend_midi_auto->isChecked())
    {
        resendMIDIControls();
        qDebug() << "RESEND MIDI CONTROLS";
    }
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



TremoloWorker::TremoloWorker(int interface_index, int delay, int channel, int note, QObject *parent)
    : QObject(parent)
{
    this->interface_index = interface_index;
    this->delay = delay;
    this->channel = channel;
    this->note = note;
    
    this->timer = new QTimer(this);
    this->timer->setInterval(delay);
    this->timer->setTimerType(Qt::PreciseTimer);
    
    connect(this->timer, &QTimer::timeout, this, &TremoloWorker::tick, Qt::DirectConnection);
    
    this->timer->start();
}

void TremoloWorker::tick()
{
    emit noteStop(this->interface_index, this->channel, this->note);
    emit notePlay(this->interface_index, this->channel, this->note);
    
    // make the tremolo effect feeling a bit more natural by not having all the time the exact same delay but just modify it a little bit
    int rnd = this->random->bounded(
                0,
                int(this->delay * .5)
                );
    int interval = this->delay + rnd;
    this->timer->setInterval(interval);
}
