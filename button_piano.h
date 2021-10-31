#ifndef BUTTONPIANO_H
#define BUTTONPIANO_H

#include <QDebug>
#include <QPushButton>
#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QPair>
#include <QKeyEvent>

class ButtonPiano : public QPushButton
{
public:
    ButtonPiano(QWidget *parent);
    
    void press();
    void release();
    
protected:
};

#endif // BUTTONPIANO_H
