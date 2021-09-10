#ifndef JACKADAPTER_H
#define JACKADAPTER_H

#include <QObject>
#include <QDebug>

#include "jack/jack.h"
#include "jack/midiport.h"

class JackAdapter : public QObject
{
    Q_OBJECT
public:
    explicit JackAdapter(QObject *parent = nullptr);
    
    void keyPressEvent(QString keycode);
    void keyReleaseEvent(QString keycode);

private:
    jack_client_t *client;
    
signals:
    
};

#endif // JACKADAPTER_H
