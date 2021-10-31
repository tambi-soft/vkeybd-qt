#ifndef INTERFACEJACK_H
#define INTERFACEJACK_H

#include <QObject>
#include <QDebug>

#include "interface_audio.h"

#include <jack/jack.h>

class InterfaceJack : public InterfaceAudio
{
    Q_OBJECT
public:
    explicit InterfaceJack(InterfaceAudio *parent = nullptr);
    
    void keyPressEvent(int midicode);
    void keyReleaseEvent(int midicode);
    
signals:
    
};

#endif // INTERFACEJACK_H
