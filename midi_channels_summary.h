#ifndef MIDICHANNELSSUMMARY_H
#define MIDICHANNELSSUMMARY_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include <QVariant>

class MIDIChannelsSummary : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIChannelsSummary(QWidget *parent = nullptr);
    
    void showData(QList<QMap<QString, QVariant> > data);
    void clearData();
    void styleLabels();
    
private:
    QGridLayout *grid;
    
signals:
    
};

#endif // MIDICHANNELSSUMMARY_H
