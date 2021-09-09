#include "keyboard_piano.h"


KeyboardPiano::KeyboardPiano(QWidget *parent) : QWidget(parent)
{
    this->setOctaves(3);
    
    for (int octaves=0; octaves < this->octaves; octaves++)
    {
        this->drawOneOctave(octaves);
    }
    
    this->size = QSize((button_width_full-1)*7*octaves, button_height_full);
}

void KeyboardPiano::setOctaves(int octaves)
{
    this->octaves = octaves;
}

void KeyboardPiano::drawOneOctave(int offset)
{
    for (int full_tones=0; full_tones < 7; full_tones++)
    {
        QPushButton *button = new ButtonPiano(this);
        button->resize(this->button_width_full, this->button_height_full);
        button->move(offset * 7*(this->button_width_full-1) + ((this->button_width_full-1) * full_tones),
                     0);
        //button->setGeometry(this->button_width_full * full_tones, 0, this->button_width_full, this->button_height_full);
        
        if (this->invert_colors == false)
        {
            this->colorizeWhiteKeys(button);
        }
        else
        {
            this->colorizeBlackKeys(button);
        }
        
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
        button->move(offset * 7*(this->button_width_full-1) + ((this->button_width_full-1) * half_tones + this->button_width_full/2 + this->button_width_full * step),
                     0);
        
        //QPalette pal = button->palette();
        //pal.setColor(QPalette::Button, QColor(Qt::black));
        //button->setPalette(pal);
        
        if (this->invert_colors == false)
        {
            this->colorizeBlackKeys(button);
        }
        else
        {
            this->colorizeWhiteKeys(button);
        }
        
        button->show();
    }
}

void KeyboardPiano::colorizeBlackKeys(QPushButton *button)
{
    QString stylesheet = "QPushButton {"
                         "  color: white;"
                         "  background-color: black;"
                         "  border-style: outset;"
                         "  border-width: 1px;"
                         "  border-radius: 0px;"
            
                         "}"
                         "QPushButton:pressed {"
                         "  color: white;"
                         "  background-color: blue;"
                         "}";
    
    button->setStyleSheet(stylesheet);
}

void KeyboardPiano::colorizeWhiteKeys(QPushButton *button)
{
    QString stylesheet = "QPushButton {"
                         "  color: black;"
                         "  background-color: white;"
                         "  border-style: outset;"
                         "  border-width: 1px;"
                         "  border-color: black;"
                         "  border-radius: 0px;"
                         "}"
                         "QPushButton:pressed {"
                         "  color: black;"
                         "  background-color: blue;"
                         "}";
    
    button->setStyleSheet(stylesheet);
}
