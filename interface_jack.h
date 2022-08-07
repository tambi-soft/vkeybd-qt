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
    
    QString label();
    
    void keyPressEvent(int channel, int midicode);
    void keyReleaseEvent(int channel, int midicode);
    void keyPanicEvent(int channel);
    void keyStopAllEvent(int channel);
    void keyPitchbendEvent(int channel, int pitch);
    void keySustainEvent(int channel, bool pressed);
    void keySostenutoEvent(int channel, bool pressed);
    void keySoftEvent(int channel, bool pressed);
    void setProgramChangeEvent(int channel, int program, int bank);
    void setVolumeChangeEvent(int channel, int volume);
    void setPanChangeEvent(int channel, int value);
    void setPortamentoChanged(int channel, int value);
    void setAttackChanged(int channel, int value);
    void setReleaseChanged(int channel, int value);
    void setTremoloChanged(int channel, int value);
    
    QString NAME = "vkeybd-qt";
    
private:
    QString label_string;
    
    jack_client_t *jack_client = NULL;
    jack_ringbuffer_t *ringbuffer;
    
    void sendEvent(bool drain);
    
signals:
    
};

#endif // INTERFACEJACK_H
