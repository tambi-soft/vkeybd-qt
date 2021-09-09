#include "keyboard_piano.h"


KeyboardPiano::KeyboardPiano(QWidget *parent) : QWidget(parent)
{
    this->drawOneOctave(0);
    this->drawOneOctave(1);
    this->drawOneOctave(2);
    
    QRect rect = this->rect();
    qDebug() << rect;
    //this->resize(rect);
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
        button->move(offset * 7*this->button_width_full + (this->button_width_full * full_tones),
                     0);
        //button->setGeometry(this->button_width_full * full_tones, 0, this->button_width_full, this->button_height_full);
        button->show();
    }
    
    for (int half_tones=0; half_tones < 5; half_tones++)
    {
        int step = 0;
        if (half_tones > 1)
        {
            step = 1;
        }
        
        QPushButton *button = new QPushButton(this);
        button->resize(this->button_width_half, this->button_height_half);
        button->move(offset * 7*this->button_width_full + (this->button_width_full * half_tones + this->button_width_full/2 + this->button_width_full * step),
                     0);
        button->show();
    }
}
