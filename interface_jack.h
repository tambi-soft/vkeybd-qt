#ifndef INTERFACEJACK_H
#define INTERFACEJACK_H

#include <QObject>
#include <QDebug>

#include "interface_audio.h"

#include <jack/jack.h>
#include <jack/midiport.h>
#include <jack/ringbuffer.h>

class InterfaceJack : public InterfaceAudio
{
    Q_OBJECT
public:
    explicit InterfaceJack(QString label, InterfaceAudio *parent = nullptr);
    ~InterfaceJack();
    
    void keyPressEvent(int midicode);
    void keyReleaseEvent(int midicode);
    void setProgramChangeEvent(int channel, int program);
    
private:
    QString label;
    jack_client_t *jack_client = NULL;
    jack_ringbuffer_t *ringbuffer;
    
signals:
    
};

#endif // INTERFACEJACK_H
