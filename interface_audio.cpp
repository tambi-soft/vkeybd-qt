#include "interface_audio.h"

InterfaceAudio::InterfaceAudio(QObject *parent) : QObject(parent)
{
    
}

void InterfaceAudio::keyPressEvent(int channel, int midicode)
{
    Q_UNUSED(channel);
    Q_UNUSED(midicode);
}

void InterfaceAudio::keyReleaseEvent(int channel, int midicode)
{
    Q_UNUSED(channel);
    Q_UNUSED(midicode);
}

void InterfaceAudio::keyPanicEvent(int channel)
{
    Q_UNUSED(channel);
}

void InterfaceAudio::keyPitchbendEvent(int channel, int pitch)
{
    Q_UNUSED(channel);
    Q_UNUSED(pitch);
}
