#include "orgelwerk.h"

Orgelwerk::Orgelwerk(QString label, QWidget *parent) : QWidget(parent)
{
    this->interface_audio = new InterfaceAlsa(label);
    //this->interface_audio = new InterfaceJack(label);
    
    drawGUI();
    initInputThread();
}

void Orgelwerk::drawGUI()
{
    this->channels = new MIDIChannelSelector(this->interface_audio);
    this->keys = new MIDIKeySelector;
    this->pitch = new MIDIPitchWheel();
    this->piano = new KeyboardPiano;
    this->pc = new KeyboardPC;
    
    connect(this->pitch, &MIDIPitchWheel::pitchWheelMoved, this, &Orgelwerk::pitchWheelMoved);
    
    connect(this->pc, &KeyboardPC::MIDIPress, this, &Orgelwerk::keyMIDIDown);
    connect(this->pc, &KeyboardPC::MIDIRelease, this, &Orgelwerk::keyMIDIUp);
    
    this->grid = new QGridLayout;
    setLayout(this->grid);
    
    QGroupBox *group_keys = new QGroupBox("Keys");
    QGroupBox *group_pitch = new QGroupBox("Pitch");
    QGroupBox *group_keyboards = new QGroupBox("Keyboards");
    
    QVBoxLayout *layout_keys = new QVBoxLayout;
    QHBoxLayout *layout_pitch = new QHBoxLayout;
    QVBoxLayout *layout_keyboards = new QVBoxLayout;
    
    group_keys->setLayout(layout_keys);
    group_pitch->setLayout(layout_pitch);
    group_keyboards->setLayout(layout_keyboards);
    
    layout_keys->addWidget(this->keys);
    layout_pitch->addWidget(this->pitch);
    layout_keyboards->addWidget(this->piano);
    layout_keyboards->addWidget(this->pc);
    
    //this->button_grab->setText("Grab Keyboard");
    //connect(this->button_grab, &QPushButton::clicked, this, &Orgelwerk::grabKeyboardPressed);
    
    this->button_panic->setText("Panic!");
    connect(this->button_panic, &QPushButton::clicked, this, &Orgelwerk::panicKeyPressed);
    QString stylesheet = "QPushButton {"
                         "  color: white;"
                         "  background-color: red;"
                         "}"
                         "QPushButton:pressed {"
                         "  color: white;"
                         "  background-color: yellow;"
                         "}";
    this->button_panic->setStyleSheet(stylesheet);
    
    QLabel *label_volume_master = new QLabel("Master Volume");
    this->slider_volume_master = new QSlider(Qt::Horizontal, this);
    this->slider_volume_master->setRange(0, 100);
    this->slider_volume_master->setValue(100);
    
    showChannelsReal(0);
    showChannelsImage(1);
    
    this->grid->addWidget(label_volume_master, 2, 0);
    this->grid->addWidget(this->slider_volume_master, 3, 0);
    this->grid->addWidget(group_keys, 4, 0);
    this->grid->addWidget(group_pitch, 5, 0);
    this->grid->addWidget(group_keyboards, 6, 0);
    this->grid->addWidget(this->button_panic, 7, 0);
    
    this->grid->setSizeConstraint( QLayout::SetFixedSize );
}

void Orgelwerk::showChannelsReal(int grid_row)
{
    QGroupBox *group_channels = new QGroupBox("Channels");
    QVBoxLayout *layout_channels = new QVBoxLayout;
    group_channels->setLayout(layout_channels);
    layout_channels->setContentsMargins(0, 0, 0, 0);
    
    QScrollArea *scroll_channels = new QScrollArea;
    scroll_channels->setWidget(this->channels);
    
    layout_channels->addWidget(scroll_channels);
    
    this->grid->addWidget(group_channels, grid_row, 0);
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
    
    this->grid->addWidget(group_channels_pixmap, grid_row, 0);
    
    updateChannelsSchreenschot();
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

void Orgelwerk::keyDown(int keycode)
{
    qDebug() << "keyDown: " + QString::number(keycode);
    this->pc->keyDown(keycode);
}
void Orgelwerk::keyUp(int keycode)
{
    qDebug() << "keyUp:   " + QString::number(keycode);
    this->pc->keyUp(keycode);
}

void Orgelwerk::panicKeyPressed()
{
    for (int i=0; i <= 127; i++)
    {
        this->piano->keyReleased(i);
    }
    
    this->pc->allKeysUp();
    
    QList<QMap<QString,int>> list_of_channels = this->channels->getListOfActivatedChannels();
    for (int c=0; c < list_of_channels.length(); c++)
    {
        this->interface_audio->keyPanicEvent(list_of_channels.at(c)["channel"]);
    }
}
void Orgelwerk::stopAllPressed()
{
    QList<QMap<QString,int>> list_of_channels = this->channels->getListOfActivatedChannels();
    for (int c=0; c < list_of_channels.length(); c++)
    {
        this->interface_audio->keyStopAllEvent(list_of_channels.at(c)["channel"]);
    }
}

void Orgelwerk::keyMIDIHelper(int midicode, QString mode)
{
    if (mode == "down")
    {
        this->piano->keyPressed(midicode);
    }
    else if (mode == "up")
    {
        this->piano->keyReleased(midicode);
    }
    
    // -12: The lower full octave on the keyboard is in the midi-range of 12 - 23 for beeing able to add some even deeper notes to the left. Therefore we get an offset of 12 we have to compensate here
    int keycode = midicode - 12;
    
    QList<QMap<QString,int>> list_of_channels = this->channels->getListOfActivatedChannels();
    
    QList<int> list_of_keys = this->keys->getListOfSelectedKeys();
    for (int k=0; k < list_of_keys.length(); k++)
    {
        for (int c=0; c < list_of_channels.length(); c++)
        {
            int channel = list_of_channels.at(c)["channel"];
            int key_shift = list_of_channels.at(c)["key_shift"];
            int key_min = list_of_channels.at(c)["key_min"];
            int key_max = list_of_channels.at(c)["key_max"]; 
            
            int m_code = keycode + list_of_keys.at(k);
            int m_code_shifted = m_code + key_shift;
            
            if ((key_min <= m_code && key_max >= m_code) || mode == "pitch")
            {
                if (mode == "down")
                {
                    this->interface_audio->keyPressEvent(channel, m_code_shifted);
                }
                else if (mode == "up")
                {
                    this->interface_audio->keyReleaseEvent(channel, m_code_shifted);
                }
                else if (mode == "pitch")
                {
                    this->interface_audio->keyPitchbendEvent(channel, midicode);
                }
            }
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
    QList<QMap<QString,int>> list_of_channels = this->channels->getListOfActivatedChannels();
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int channel = list_of_channels.at(c)["channel"];
        this->interface_audio->keySustainEvent(channel, pressed);
    }
}

void Orgelwerk::keySostenuto(bool pressed)
{
    QList<QMap<QString,int>> list_of_channels = this->channels->getListOfActivatedChannels();
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int channel = list_of_channels.at(c)["channel"];
        this->interface_audio->keySostenutoEvent(channel, pressed);
    }
}

void Orgelwerk::keySoft(bool pressed)
{
    QList<QMap<QString,int>> list_of_channels = this->channels->getListOfActivatedChannels();
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int channel = list_of_channels.at(c)["channel"];
        int volume = list_of_channels.at(c)["volume"];
        this->interface_audio->keySoftEvent(channel, pressed, volume);
    }
}

void Orgelwerk::showChannelDetails(bool update_preview)
{
    QDialog *dialog = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    layout_dialog->addWidget(this->channels);
    
    dialog->layout()->setSizeConstraint( QLayout::SetFixedSize );
    
    dialog->exec();
    
    if (update_preview)
    {
        updateChannelsSchreenschot();
    }
    
}

void Orgelwerk::updateChannelsSchreenschot()
{
    QPixmap channels_pixmap = this->channels->grab();
    /*
    QRect rect;
    rect.setWidth(600);
    this->button_channels->setGeometry(rect);
    */
    
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
