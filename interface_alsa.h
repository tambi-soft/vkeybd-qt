#ifndef INTERFACEALSA_H
#define INTERFACEALSA_H

#include <QObject>
#include <QDebug>

#include "interface_audio.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <alsa/asoundlib.h>
#include <alsa/control.h>
#include <alsa/seq.h>

/*
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include </usr/include/alsa/asoundlib.h>
#include </usr/include/alsa/pcm.h>
*/

class InterfaceAlsa : public InterfaceAudio
{
    Q_OBJECT
public:
    explicit InterfaceAlsa(QString label, InterfaceAudio *parent = nullptr);
    ~InterfaceAlsa();
    
    void keyPressEvent(int channel, int midicode);
    void keyReleaseEvent(int channel, int midicode);
    void keyPanicEvent(int channel);
    void keyStopAllEvent(int channel);
    void keyPitchbendEvent(int channel, int pitch);
    void keySustainEvent(int channel, bool pressed);
    void keySostenutoEvent(int channel, bool pressed);
    void keySoftEvent(int channel, bool pressed, int volume);
    void setProgramChangeEvent(int channel, int program, int bank);
    void setVolumeChangeEvent(int channel, int volume);
    void setPanChangeEvent(int channel, int value);
    void setPortamentoChanged(int channel, int value);
    void setAttackChanged(int channel, int value);
    void setReleaseChanged(int channel, int value);
    
    QString NAME = "vkeybd-qt";
    
private:
    QString label;
    snd_seq_t *seq;
    snd_seq_event_t ev;
    
    void sendEvent(bool drain);
    
signals:
    
};

#endif // INTERFACEALSA_H
