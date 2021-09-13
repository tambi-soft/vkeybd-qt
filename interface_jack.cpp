#include "interface_jack.h"

InterfaceJack::InterfaceJack(InterfaceAudio *parent) : InterfaceAudio(parent)
{
    
}

void InterfaceJack::keyPressEvent(QString keycode)
{
    qDebug() << "jack pressed: "+keycode;
}

void InterfaceJack::keyReleaseEvent(QString keycode)
{
    qDebug() << "jack released: "+keycode;
}
