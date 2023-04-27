#include "interface_audio.h"

InterfaceAudio::InterfaceAudio(QObject *parent) : QObject(parent)
{
    
}

InterfaceAudio::~InterfaceAudio()
{
    
}

void InterfaceAudio::createNewPort(QString label)
{
    Q_UNUSED(label);
}

QString InterfaceAudio::label()
{
    return "";
}

void InterfaceAudio::keyPressEvent(int port, int channel, int midicode, int velocity)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(midicode);
    Q_UNUSED(velocity);
}

void InterfaceAudio::keyReleaseEvent(int port, int channel, int midicode, int velocity)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(midicode);
    Q_UNUSED(velocity);
}

void InterfaceAudio::keyPanicEvent(int port, int channel)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
}

void InterfaceAudio::keyStopAllEvent(int port, int channel)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
}

void InterfaceAudio::keyPitchbendEvent(int port, int channel, int pitch)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(pitch);
}

void InterfaceAudio::keySustainEvent(int port, int channel, bool pressed)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceAudio::keySostenutoEvent(int port, int channel, bool pressed)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceAudio::keySoftEvent(int port, int channel, bool pressed)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceAudio::setProgramChangeEvent(int port, int channel, int program, int bank)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(program);
    Q_UNUSED(bank);
}

void InterfaceAudio::setVolumeChangeEvent(int port, int channel, int volume)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(volume);
}

void InterfaceAudio::setPanChangeEvent(int port, int channel, int value)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceAudio::setPortamentoChanged(int port, int channel, int value)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceAudio::setAttackChanged(int port, int channel, int value)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceAudio::setReleaseChanged(int port, int channel, int value)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceAudio::setTremoloChanged(int port, int channel, int value)
{
    Q_UNUSED(port);
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceAudio::saveMIDISettings()
{
    
}
void InterfaceAudio::loadMIDISettings()
{
     
}
