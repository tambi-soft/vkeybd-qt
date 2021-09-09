#include "button_piano.h"

ButtonPiano::ButtonPiano(QString note, int octave, QWidget *parent)
    :QPushButton(parent)
{
    this->note = note;
    this->octave = octave;
    
    this->installEventFilter(this);
}

bool ButtonPiano::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::MouseButtonPress)
    {
        qDebug() << this->note << " " << this->octave << " pressed";
        
        this->jack->keyPressEvent(this->note);
    }
    else if (ev->type() == QEvent::MouseButtonRelease)
    {
        qDebug() << this->note << " " << this->octave << " released";
        
        this->jack->keyReleaseEvent(this->note);
    }
    return QObject::eventFilter(obj, ev);
}
