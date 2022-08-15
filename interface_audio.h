#ifndef INTERFACEAUDIO_H
#define INTERFACEAUDIO_H

#include <QObject>
#include <QMap>

class InterfaceAudio : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceAudio(QObject *parent = nullptr);
    ~InterfaceAudio();
    
    virtual QString label();
    
    virtual void createNewPort(QString label);
    
    virtual void keyPressEvent(int port, int channel, int midicode);
    virtual void keyReleaseEvent(int port, int channel, int midicode);
    virtual void keyPanicEvent(int port, int channel);
    virtual void keyStopAllEvent(int port, int channel);
    virtual void keyPitchbendEvent(int port, int channel, int pitch);
    virtual void keySustainEvent(int port, int channel, bool pressed);
    virtual void keySostenutoEvent(int port, int channel, bool pressed);
    virtual void keySoftEvent(int port, int channel, bool pressed);
    virtual void setProgramChangeEvent(int port, int channel, int program, int bank);
    virtual void setVolumeChangeEvent(int port, int channel, int volume);
    virtual void setPanChangeEvent(int port, int channel, int value);
    virtual void setPortamentoChanged(int port, int channel, int value);
    virtual void setAttackChanged(int port, int channel, int value);
    virtual void setReleaseChanged(int port, int channel, int value);
    virtual void setTremoloChanged(int port, int channel, int value);
    
signals:
    
};

#endif // INTERFACEAUDIO_H
