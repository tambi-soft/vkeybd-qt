#ifndef BUTTONPIANO_H
#define BUTTONPIANO_H

#include <QDebug>
#include <QPushButton>
#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QPair>
#include <QKeyEvent>

#include "jack_adapter.h"

class ButtonPiano : public QPushButton
{
public:
    ButtonPiano(JackAdapter *jack, QString keycode, QWidget *parent);
    
    JackAdapter *jack;
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
