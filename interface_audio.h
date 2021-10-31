#ifndef INTERFACEAUDIO_H
#define INTERFACEAUDIO_H

#include <QObject>

class InterfaceAudio : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceAudio(QObject *parent = nullptr);
    
    virtual void keyPressEvent(int midicode);
    virtual void keyReleaseEvent(int midicode);
    virtual void keyPanicEvent();
    virtual void keyPitchbendEvent(int pitch);
    
signals:
    
};

#endif // INTERFACEAUDIO_H
