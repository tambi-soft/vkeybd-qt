#ifndef INTERFACEAUDIO_H
#define INTERFACEAUDIO_H

#include <QObject>

class InterfaceAudio : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceAudio(QObject *parent = nullptr);
    
    virtual void keyPressEvent(int channel, int midicode);
    virtual void keyReleaseEvent(int channel, int midicode);
    virtual void keyPanicEvent(int channel);
    virtual void keyPitchbendEvent(int channel, int pitch);
    
signals:
    
};

#endif // INTERFACEAUDIO_H
