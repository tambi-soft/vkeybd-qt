#include "button_piano.h"

ButtonPiano::ButtonPiano(JackAdapter *jack, QString keycode, QWidget *parent)
    :QPushButton(parent)
{
    this->jack = jack;
    this->keycode = keycode;
}

void ButtonPiano::press()
{
    this->setDown(true);
}

void ButtonPiano::release()
{
    this->setDown(false);
}
