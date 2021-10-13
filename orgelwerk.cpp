#include "orgelwerk.h"

Orgelwerk::Orgelwerk(QWidget *parent) : QWidget(parent)
{
    this->channels = new MIDIChannelSelector;
    this->keys = new MIDIKeySelector;
    this->piano = new KeyboardPiano;
    this->pc = new KeyboardPC;
    
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    
    QGroupBox *group_channels = new QGroupBox("MIDI Channels");
    QGroupBox *group_keys = new QGroupBox("MIDI Keys");
    QGroupBox *group_keyboard_piano = new QGroupBox("Keyboard Piano");
    QGroupBox *group_keyboard_pc = new QGroupBox("Keyboard PC");
    
    QVBoxLayout *layout_channels = new QVBoxLayout;
    QVBoxLayout *layout_keys = new QVBoxLayout;
    QVBoxLayout *layout_keyboard_piano = new QVBoxLayout;
    QVBoxLayout *layout_keyboard_pc = new QVBoxLayout;
    
    group_channels->setLayout(layout_channels);
    group_keys->setLayout(layout_keys);
    group_keyboard_piano->setLayout(layout_keyboard_piano);
    group_keyboard_pc->setLayout(layout_keyboard_pc);
    
    layout_channels->addWidget(this->channels);
    layout_keys->addWidget(this->keys);
    layout_keyboard_piano->addWidget(this->piano);
    layout_keyboard_pc->addWidget(this->pc);
    
    //layout->addWidget(this->channels);
    //layout->addWidget(this->keys);
    //layout->addWidget(this->piano);
    layout->addWidget(group_channels);
    layout->addWidget(group_keys);
    layout->addWidget(group_keyboard_piano);
    layout->addWidget(group_keyboard_pc);
}

