#include "interface_jack.h"

InterfaceJack::InterfaceJack(InterfaceAudio *parent) : InterfaceAudio(parent)
{
    
}

void InterfaceJack::keyPressEvent(int midicode)
{
    qDebug() << "jack pressed: "+QString::number(midicode);
}

void InterfaceJack::keyReleaseEvent(int midicode)
{
    qDebug() << "jack released: "+QString::number(midicode);
}
