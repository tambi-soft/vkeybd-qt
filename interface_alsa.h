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
    
    void keyPressEvent(int midicode);
    void keyReleaseEvent(int midicode);
    
    QString NAME = "vkeybd-qt";
    
private:
    snd_seq_t *seq;
    
signals:
    
};

#endif // INTERFACEALSA_H
