#ifndef BUTTONPIANO_H
#define BUTTONPIANO_H

#include <QDebug>
#include <QPushButton>
#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QPair>
#include <QKeyEvent>

#include "interface_audio.h"
#include "interface_alsa.h"
#include "interface_jack.h"

class ButtonPiano : public QPushButton
{
public:
    ButtonPiano(InterfaceAudio *interface_audio, QString keycode, QWidget *parent);
    
    InterfaceAudio *interface_audio;
    QString keycode;
    
    /*
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);
    */
    void press();
    void release();
    
protected:
};

#endif // BUTTONPIANO_H
