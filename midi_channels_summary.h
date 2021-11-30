#ifndef MIDICHANNELSSUMMARY_H
#define MIDICHANNELSSUMMARY_H

#include <QObject>
#include <QWidget>

class MIDIChannelsSummary : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIChannelsSummary(QWidget *parent = nullptr);
    
    void showData(QList<QMap<QString,int>> data);
    
signals:
    
};

#endif // MIDICHANNELSSUMMARY_H
