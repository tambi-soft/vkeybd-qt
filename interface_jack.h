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
    
    void keyPressEvent(QString keycode);
    void keyReleaseEvent(QString keycode);
    
signals:
    
};

#endif // INTERFACEJACK_H
