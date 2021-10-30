#include "button_piano.h"

ButtonPiano::ButtonPiano(InterfaceAudio *interface_audio, QWidget *parent)
    :QPushButton(parent)
{
    this->interface_audio = interface_audio;
}

void ButtonPiano::press()
{
    //this->interface_audio->keyPressEvent(this->keycode);
    this->setDown(true);
}

void ButtonPiano::release()
{
    //this->interface_audio->keyReleaseEvent(this->keycode);
    this->setDown(false);
}
