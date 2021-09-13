#ifndef ALSAADAPTER_H
#define ALSAADAPTER_H

#include <QObject>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

class AlsaAdapter : public QObject
{
    Q_OBJECT
public:
    explicit AlsaAdapter(QObject *parent = nullptr);
    
signals:
    
};

#endif // ALSAADAPTER_H
