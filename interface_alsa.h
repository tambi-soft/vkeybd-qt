#ifndef INTERFACEALSA_H
#define INTERFACEALSA_H

#include <QObject>
#include <QDebug>

#include "interface_audio.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include </usr/include/alsa/asoundlib.h>
#include <alsa/asoundlib.h>
//#include </usr/include/alsa/control.h>
#include <alsa/control.h>
//#include </usr/include/alsa/seq.h>
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
    
    void keyPressEvent(int channel, int midicode);
    void keyReleaseEvent(int channel, int midicode);
    void keyPanicEvent(int channel);
    void keyPitchbendEvent(int channel, int pitch);
    
    QString NAME = "vkeybd-qt";
    
private:
    QString label;
    snd_seq_t *seq;
    snd_seq_event_t ev;
    
signals:
    
};

#endif // INTERFACEALSA_H
