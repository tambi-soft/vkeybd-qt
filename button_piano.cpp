#include "button_piano.h"

ButtonPiano::ButtonPiano(QWidget *parent)
    :QPushButton(parent)
{
    
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
