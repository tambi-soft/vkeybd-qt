#include "keyboard_piano.h"


KeyboardPiano::KeyboardPiano(QWidget *parent) : QWidget(parent)
{
    this->drawOneOctave(0);
}

void KeyboardPiano::setOctaves(int octaves)
{
    this->octaves = octaves;
}

void KeyboardPiano::drawOneOctave(int offset)
{
    for (int full_tones=0; full_tones < 7; full_tones++)
    {
        QPushButton *button = new QPushButton(this);
        button->resize(this->button_width_full, this->button_height_full);
        button->move(this->button_width_full * full_tones, 0);
        //button->setGeometry(this->button_width_full * full_tones, 0, this->button_width_full, this->button_height_full);
        button->show();
    }
    
    for (int half_tones=0; half_tones < 5; half_tones++)
    {
        
    }
}
