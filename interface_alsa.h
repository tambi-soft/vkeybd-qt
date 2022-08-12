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
    explicit InterfaceAlsa(InterfaceAudio *parent = nullptr);
    ~InterfaceAlsa();
    
    void createNewPort(QString label);
    QMap<int,QString> getPorts();
    QMap<int,QString> getLastCreatedPort();
    
    void keyPressEvent(int port, int channel, int midicode);
    void keyReleaseEvent(int port, int channel, int midicode);
    void keyPanicEvent(int port, int channel);
    void keyStopAllEvent(int port, int channel);
    void keyPitchbendEvent(int port, int channel, int pitch);
    void keySustainEvent(int port, int channel, bool pressed);
    void keySostenutoEvent(int port, int channel, bool pressed);
    void keySoftEvent(int port, int channel, bool pressed);
    void setProgramChangeEvent(int port, int channel, int program, int bank);
    void setVolumeChangeEvent(int port, int channel, int volume);
    void setPanChangeEvent(int port, int channel, int value);
    void setPortamentoChanged(int port, int channel, int value);
    void setAttackChanged(int port, int channel, int value);
    void setReleaseChanged(int port, int channel, int value);
    void setTremoloChanged(int port, int channel, int value);
    
    QString NAME = "vkeybd-qt";
    
private:
    snd_seq_t *seq;
    snd_seq_event_t ev;
    
    QMap<int,QString> ports;
    QMap<int,QString> port_last;
    
    void sendEvent(bool drain);
    
signals:
    
};

#endif // INTERFACEALSA_H
