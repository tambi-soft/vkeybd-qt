#include "orgelwerk.h"

Orgelwerk::Orgelwerk(QString label, QWidget *parent) : QWidget(parent)
{
    this->interface_audio = new InterfaceAlsa(label);
    //this->interface_audio = new InterfaceJack;
    
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
    
    connect(this->channels, &MIDIChannelSelector::volumeChangedSignal, this, &Orgelwerk::volumeChanged);
    connect(this->channels, &MIDIChannelSelector::instrumentChangedSignal, this, &Orgelwerk::instrumentChanged);
    
    connect(this->pitch, &MIDIPitchWheel::pitchWheelMoved, this, &Orgelwerk::pitchWheelMoved);
    
    connect(this->pc, &KeyboardPC::MIDIPress, this, &Orgelwerk::keyMIDIDown);
    connect(this->pc, &KeyboardPC::MIDIRelease, this, &Orgelwerk::keyMIDIUp);
    
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    
    QGroupBox *group_channels = new QGroupBox("Channels");
    QGroupBox *group_keys = new QGroupBox("Keys");
    QGroupBox *group_pitch = new QGroupBox("Pitch");
    QGroupBox *group_keyboards = new QGroupBox("Keyboards");
    
    QVBoxLayout *layout_channels = new QVBoxLayout;
    QVBoxLayout *layout_keys = new QVBoxLayout;
    QHBoxLayout *layout_pitch = new QHBoxLayout;
    QVBoxLayout *layout_keyboards = new QVBoxLayout;
    
    layout_channels->setContentsMargins(0, 0, 0, 0);
    
    group_channels->setLayout(layout_channels);
    group_keys->setLayout(layout_keys);
    group_pitch->setLayout(layout_pitch);
    group_keyboards->setLayout(layout_keyboards);
    
    QScrollArea *scroll_channels = new QScrollArea;
    scroll_channels->setWidget(this->channels);
    
    layout_channels->addWidget(scroll_channels);
    layout_keys->addWidget(this->keys);
    //layout_program->addWidget();
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
    
    layout->addWidget(group_channels);
    layout->addWidget(group_keys);
    layout->addWidget(group_pitch);
    layout->addWidget(group_keyboards);
    //layout->addWidget(this->button_grab);
    layout->addWidget(this->button_panic);
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
    qDebug() << "keyUp: " + QString::number(keycode);
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

void Orgelwerk::volumeChanged(int channel, int volume)
{
    this->interface_audio->setVolumeChangeEvent(channel, volume);
}

void Orgelwerk::instrumentChanged(int channel, int program, int bank)
{
    this->interface_audio->setProgramChangeEvent(channel, program, bank);
}
