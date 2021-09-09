#ifndef BUTTONPIANO_H
#define BUTTONPIANO_H

#include <QDebug>
#include <QPushButton>
#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QPair>

#include "jack_adapter.h"

class ButtonPiano : public QPushButton
{
public:
    ButtonPiano(QString note, int octave, QWidget *parent);
    
    QString note;
    QString octave;
    
protected:
    JackAdapter *jack = new JackAdapter;
    bool eventFilter(QObject *obj, QEvent *ev);
};

#endif // BUTTONPIANO_H
