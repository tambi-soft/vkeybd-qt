#include "interface_audio.h"

InterfaceAudio::InterfaceAudio(QObject *parent) : QObject(parent)
{
    
}

void InterfaceAudio::keyPressEvent(int midicode)
{
    Q_UNUSED(midicode);
}

void InterfaceAudio::keyReleaseEvent(int midicode)
{
    Q_UNUSED(midicode);
}
