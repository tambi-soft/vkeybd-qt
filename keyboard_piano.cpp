#include "keyboard_piano.h"


KeyboardPiano::KeyboardPiano(QWidget *parent) : QWidget(parent)
{
    this->setOctaves(3);
    
    //this->interface_audio = new InterfaceAlsa;
    this->interface_audio = new InterfaceJack;
    
    for (int octave=0; octave < this->octaves; octave++)
    {
        this->drawOneOctave(octave);
    }
    
    this->size = QSize((button_width_full-1)*7*octaves, button_height_full-1);
    
    qDebug() << this->list_of_keys;
}

void KeyboardPiano::setOctaves(int octaves)
{
    this->octaves = octaves;
}

void KeyboardPiano::drawOneOctave(int octave)
{
    for (int full_tone=0; full_tone < 7; full_tone++)
    {
        QString keycode = this->notes_full[full_tone] + QString::number(octave);
        
        ButtonPiano *button = new ButtonPiano(this->interface_audio, keycode, this);
        button->resize(this->button_width_full, this->button_height_full);
        button->move(octave * 7*(this->button_width_full-1) + ((this->button_width_full-1) * full_tone),
                     -1);
        
        if (this->invert_colors == false)
        {
            this->colorizeWhiteKeys(button);
        }
        else
        {
            this->colorizeBlackKeys(button);
        }
        
        button->show();
        
        this->list_of_keys[keycode] = button;
    }
    
    for (int half_tone=0; half_tone < 5; half_tone++)
    {
        QString keycode = this->notes_half[half_tone] + QString::number(octave);
        
        int step = 0;
        if (half_tone > 1)
        {
            step = 1;
        }
        
        ButtonPiano *button = new ButtonPiano(this->interface_audio, keycode, this);
        button->resize(this->button_width_half, this->button_height_half);
        button->move(octave * 7*(this->button_width_full-1) + ((this->button_width_full-1) * half_tone + this->button_width_full/2 + this->button_width_full * step),
                     -1);
        
        if (this->invert_colors == false)
        {
            this->colorizeBlackKeys(button);
        }
        else
        {
            this->colorizeWhiteKeys(button);
        }
        
        button->show();
        
        this->list_of_keys[keycode] = button;
    }
}

void KeyboardPiano::colorizeBlackKeys(ButtonPiano *button)
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

void KeyboardPiano::colorizeWhiteKeys(ButtonPiano *button)
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

void KeyboardPiano::keyPressed(QString keycode)
{
    ButtonPiano *button = this->list_of_keys[keycode];
    button->press();
}

void KeyboardPiano::keyReleased(QString keycode)
{
    ButtonPiano *button = this->list_of_keys[keycode];
    button->release();
}
