#ifndef JACKADAPTER_H
#define JACKADAPTER_H

#include <QObject>

#include "jack/jack.h"
#include "jack/midiport.h"

class JackAdapter : public QObject
{
    Q_OBJECT
public:
    explicit JackAdapter(QObject *parent = nullptr);
    
    void keyPressEvent(QString note);
    void keyReleaseEvent(QString note);
    
signals:
    
};

#endif // JACKADAPTER_H
