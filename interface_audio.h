#ifndef INTERFACEAUDIO_H
#define INTERFACEAUDIO_H

#include <QObject>

class InterfaceAudio : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceAudio(QObject *parent = nullptr);
    ~InterfaceAudio();
    
    virtual void keyPressEvent(int channel, int midicode);
    virtual void keyReleaseEvent(int channel, int midicode);
    virtual void keyPanicEvent(int channel);
    virtual void keyPitchbendEvent(int channel, int pitch);
    virtual void setProgramChangeEvent(int channel, int program, int bank);
    virtual void setVolumeChangeEvent(int channel, int volume);
    virtual void setPortamentoChanged(int channel, int value);
    virtual void setAttackChanged(int channel, int value);
    virtual void setReleaseChanged(int channel, int value);
    
signals:
    
};

#endif // INTERFACEAUDIO_H
