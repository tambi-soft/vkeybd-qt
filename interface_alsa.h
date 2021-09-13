#ifndef INTERFACEALSA_H
#define INTERFACEALSA_H

#include <QObject>
#include <QDebug>

#include "interface_audio.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

class InterfaceAlsa : public InterfaceAudio
{
    Q_OBJECT
public:
    explicit InterfaceAlsa(InterfaceAudio *parent = nullptr);
    
    void keyPressEvent(QString keycode);
    void keyReleaseEvent(QString keycode);
    
signals:
    
};

#endif // INTERFACEALSA_H
