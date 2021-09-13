#include "button_piano.h"

ButtonPiano::ButtonPiano(InterfaceAudio *interface_audio, QString keycode, QWidget *parent)
    :QPushButton(parent)
{
    this->interface_audio = interface_audio;
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
    this->interface_audio->keyPressEvent(this->keycode);
    this->setDown(true);
}

void ButtonPiano::release()
{
    this->interface_audio->keyReleaseEvent(this->keycode);
    this->setDown(false);
}
