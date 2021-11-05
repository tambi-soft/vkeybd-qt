#include "interface_jack.h"

InterfaceJack::InterfaceJack(InterfaceAudio *parent) : InterfaceAudio(parent)
{
    
}

InterfaceJack::~InterfaceJack()
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

void InterfaceJack::setProgramChangeEvent(int channel, int program)
{
    Q_UNUSED(channel);
    Q_UNUSED(program);
}
