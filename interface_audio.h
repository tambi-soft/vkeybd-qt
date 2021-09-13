#ifndef INTERFACEAUDIO_H
#define INTERFACEAUDIO_H

#include <QObject>

class InterfaceAudio : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceAudio(QObject *parent = nullptr);
    
    void keyPressEvent(QString keycode);
    void keyReleaseEvent(QString keycode);
    
signals:
    
};

#endif // INTERFACEAUDIO_H
