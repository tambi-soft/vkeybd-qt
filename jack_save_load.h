#ifndef JACKSAVELOAD_H
#define JACKSAVELOAD_H

#include <QObject>

#include <jack/jack.h>
#include <jack/midiport.h>

class JACKSaveLoad : public QObject
{
    Q_OBJECT
public:
    explicit JACKSaveLoad(QObject *parent = nullptr);
    
signals:
    
};

#endif // JACKSAVELOAD_H
