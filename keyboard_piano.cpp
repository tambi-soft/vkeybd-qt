#include "keyboard_piano.h"


KeyboardPiano::KeyboardPiano(QWidget *parent) : QWidget(parent)
{
    this->setOctaves(3);
    
    for (int octave=0; octave < this->octaves; octave++)
    {
        this->drawOneOctave(octave);
    }
    
    this->size = QSize((button_width_full-1)*7*octaves+1, button_height_full-1);
    
    setMinimumSize(this->size);
    setMaximumSize(this->size);
    
    qDebug() << this->size;
    //qDebug() << this->list_of_keys;
}

void KeyboardPiano::setOctaves(int octaves)
{
    this->octaves = octaves;
}

void KeyboardPiano::drawOneOctave(int octave)
{
    QList<int> midi_codes_full = {12, 14, 16, 17, 19, 21, 23};
    QList<int> midi_codes_half = {13, 15, 18, 20, 22};
    
    for (int full_tone=0; full_tone < 7; full_tone++)
    {
        ButtonPiano *button = new ButtonPiano(this);
        button->resize(this->button_width_full, this->button_height_full);
        button->move(
                        octave * 7*(this->button_width_full-1) + ((this->button_width_full-1) * full_tone),
                        -1
                    );
        
        if (this->invert_colors == false)
        {
            this->colorizeWhiteKeys(button);
        }
        else
        {
            this->colorizeBlackKeys(button);
        }
        
        button->show();
        
        int code = midi_codes_full.at(full_tone);
        this->list_of_keys_midi[code + (octave * 12)] = button;
    }
    
    for (int half_tone=0; half_tone < 5; half_tone++)
    {
        int step = 0;
        if (half_tone > 1)
        {
            step = 1;
        }
        
        ButtonPiano *button = new ButtonPiano(this);
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
        
        int code = midi_codes_half.at(half_tone);
        this->list_of_keys_midi[code + (octave * 12)] = button;
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

void KeyboardPiano::keyPressed(int midicode)
{
    if (this->list_of_keys_midi.contains(midicode))
    {
        ButtonPiano *button = this->list_of_keys_midi[midicode];
        
        button->press();
    }
}

void KeyboardPiano::keyReleased(int midicode)
{
    if (this->list_of_keys_midi.contains(midicode))
    {
        ButtonPiano *button = this->list_of_keys_midi[midicode];
        
        button->release();
    }
}
