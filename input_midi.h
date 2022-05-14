#ifndef INPUTMIDI_H
#define INPUTMIDI_H

#include <QObject>

class InputMIDI : public QObject
{
    Q_OBJECT
public:
    explicit InputMIDI(QObject *parent = nullptr);
    
signals:
    
};

#endif // INPUTMIDI_H
