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
    
    QList<QString> getInstrumentGroups();
    QList<QString> getInstrumentsForGroupMIDIv1(QString group);
    QList<QString> getInstrumentsForGroupMIDIv2(QString group);
    QMap<QString, int> getMIDICodesForInstrument(QString instrument);
    QMap<QString, QString> getInstrumentForMIDICodes(int msb, int lsb);
    
    QList<QString> getNuanceVelocities();
    
    //QString BANK_NO_NAME = "[None]";
    
private:
    QList<QString> list_of_groups;
    QList<QVariant> splitLine(QString line);
    QList<QString> getInstrumentsRAWData();
    QList<QString> getInstrumentsForGroupHelper(QString group, bool midi_v2);
};

#endif // MIDISOUNDSLIST_H
