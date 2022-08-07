#include "interface_audio.h"

InterfaceAudio::InterfaceAudio(QString label, QObject *parent) : QObject(parent)
{
    Q_UNUSED(label);
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

void InterfaceAudio::keyStopAllEvent(int channel)
{
    Q_UNUSED(channel);
}

void InterfaceAudio::keyPitchbendEvent(int channel, int pitch)
{
    Q_UNUSED(channel);
    Q_UNUSED(pitch);
}

void InterfaceAudio::keySustainEvent(int channel, bool pressed)
{
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceAudio::keySostenutoEvent(int channel, bool pressed)
{
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceAudio::keySoftEvent(int channel, bool pressed)
{
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceAudio::setProgramChangeEvent(int channel, int program, int bank)
{
    Q_UNUSED(channel);
    Q_UNUSED(program);
    Q_UNUSED(bank);
}

void InterfaceAudio::setVolumeChangeEvent(int channel, int volume)
{
    Q_UNUSED(channel);
    Q_UNUSED(volume);
}

void InterfaceAudio::setPanChangeEvent(int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceAudio::setPortamentoChanged(int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceAudio::setAttackChanged(int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceAudio::setReleaseChanged(int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceAudio::setTremoloChanged(int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}
