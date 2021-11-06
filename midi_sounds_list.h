#ifndef MIDISOUNDSLIST_H
#define MIDISOUNDSLIST_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QDebug>

class MIDISoundsList
{
public:
    MIDISoundsList();
    
    static QStringList getCategories();
    
    QList<QString> getInstrumentGroups();
    QList<QString> getInstrumentsForGroupMIDIv1(QString group);
    QList<QString> getInstrumentsForGroupMIDIv2(QString group);
    QList<int>  getMIDICodesForInstrument(QString instrument);
    
private:
    QList<QString> list_of_groups;
    QList<QVariant> splitLine(QString line);
    QList<QString> getInstrumentsRAWData();
    QList<QString> getInstrumentsForGroupHelper(QString group, bool midi_v2);
};

#endif // MIDISOUNDSLIST_H
