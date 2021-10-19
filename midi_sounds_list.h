#ifndef MIDISOUNDSLIST_H
#define MIDISOUNDSLIST_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QVector3D>

class MIDISoundsList
{
public:
    MIDISoundsList();
    
    static QStringList getCategories();
    void getInstruments();
};

#endif // MIDISOUNDSLIST_H
