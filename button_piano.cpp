#include "button_piano.h"

ButtonPiano::ButtonPiano(JackAdapter *jack, QString keycode, QWidget *parent)
    :QPushButton(parent)
{
    this->jack = jack;
    this->keycode = keycode;
}

/*
void ButtonPiano::keyPressEvent(QKeyEvent *ev)
{
    this->press();
}

void ButtonPiano::keyReleaseEvent(QKeyEvent *ev)
{
    this->release();
}
*/

void ButtonPiano::press()
{
    this->jack->keyPressEvent(this->keycode);
    this->setDown(true);
}

void ButtonPiano::release()
{
    this->jack->keyReleaseEvent(this->keycode);
    this->setDown(false);
}
