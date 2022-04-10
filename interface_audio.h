#ifndef INTERFACEAUDIO_H
#define INTERFACEAUDIO_H

#include <QObject>

class InterfaceAudio : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceAudio(QString label, QObject *parent = nullptr);
    ~InterfaceAudio();
    
    virtual QString label();
    
    virtual void keyPressEvent(int channel, int midicode);
    virtual void keyReleaseEvent(int channel, int midicode);
    virtual void keyPanicEvent(int channel);
    virtual void keyStopAllEvent(int channel);
    virtual void keyPitchbendEvent(int channel, int pitch);
    virtual void keySustainEvent(int channel, bool pressed);
    virtual void keySostenutoEvent(int channel, bool pressed);
    virtual void keySoftEvent(int channel, bool pressed, int volume);
    virtual void setProgramChangeEvent(int channel, int program, int bank);
    virtual void setVolumeChangeEvent(int channel, int volume);
    virtual void setPanChangeEvent(int channel, int value);
    virtual void setPortamentoChanged(int channel, int value);
    virtual void setAttackChanged(int channel, int value);
    virtual void setReleaseChanged(int channel, int value);
    virtual void setTremoloChanged(int channel, int value);
    
signals:
    
};

#endif // INTERFACEAUDIO_H
