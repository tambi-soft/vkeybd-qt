#include "orgelwerk.h"

Orgelwerk::Orgelwerk(QWidget *parent) : QWidget(parent)
{
    this->channels = new MIDIChannelSelector;
    this->keys = new MIDIKeySelector;
    this->pitch = new MIDIPitchWheel;
    this->piano = new KeyboardPiano;
    this->pc = new KeyboardPC;
    
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    
    QGroupBox *group_channels = new QGroupBox("Channels");
    QGroupBox *group_keys = new QGroupBox("Keys");
    QGroupBox *group_program = new QGroupBox("Program List");
    QGroupBox *group_pitch = new QGroupBox("Pitch");
    QGroupBox *group_keyboards = new QGroupBox("Keyboards");
    
    QVBoxLayout *layout_channels = new QVBoxLayout;
    QVBoxLayout *layout_keys = new QVBoxLayout;
    QHBoxLayout *layout_program = new QHBoxLayout;
    QHBoxLayout *layout_pitch = new QHBoxLayout;
    QVBoxLayout *layout_keyboards = new QVBoxLayout;
    
    group_channels->setLayout(layout_channels);
    group_keys->setLayout(layout_keys);
    group_program->setLayout(layout_program);
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
    
    layout->addWidget(group_channels);
    layout->addWidget(group_keys);
    layout->addWidget(group_program);
    layout->addWidget(group_pitch);
    layout->addWidget(group_keyboards);
}
