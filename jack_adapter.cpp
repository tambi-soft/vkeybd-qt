#include "jack_adapter.h"

JackAdapter::JackAdapter(QObject *parent) : QObject(parent)
{
    
}

void JackAdapter::keyPressEvent(QString keycode)
{
    qDebug() << "jack pressed: "+keycode;
}

void JackAdapter::keyReleaseEvent(QString keycode)
{
    qDebug() << "jack released: "+keycode;
}
