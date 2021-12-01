#include "midi_channels_summary.h"

MIDIChannelsSummary::MIDIChannelsSummary(QWidget *parent) : QWidget(parent)
{
    this->grid = new QGridLayout;
    setLayout(this->grid);
}

void MIDIChannelsSummary::showData(QList<QMap<QString, int>> data)
{
    clearData();
    
    QLabel *label_channel_number = new QLabel("<b>Channel</b>");
    QLabel *label_volume = new QLabel("<b>Volume</b>");
    QLabel *label_key_shift = new QLabel("<b>Key Shift</b>");
    QLabel *label_key_min = new QLabel("<b>Key Min</b>");
    QLabel *label_key_max = new QLabel("<b>Key Max</b>");
    
    this->grid->addWidget(label_channel_number, 0, 0);
    this->grid->addWidget(label_volume, 0, 1);
    this->grid->addWidget(label_key_shift, 0, 2);
    this->grid->addWidget(label_key_min, 0, 3);
    this->grid->addWidget(label_key_max, 0, 4);
    
    for (int i=0; i < data.length(); i++)
    {
        QLabel *label_channel_ = new QLabel(QString::number(data.at(i)["channel"]+1));
        QLabel *label_volume_ = new QLabel(QString::number(data.at(i)["volume"]));
        QLabel *label_key_shift_ = new QLabel(QString::number(data.at(i)["key_shift"]));
        QLabel *label_key_min_ = new QLabel(QString::number(data.at(i)["key_min"]));
        QLabel *label_key_max_ = new QLabel(QString::number(data.at(i)["key_max"]));
        
        int j = i+1;
        
        this->grid->addWidget(label_channel_, j, 0);
        this->grid->addWidget(label_volume_, j, 1);
        this->grid->addWidget(label_key_shift_, j, 2);
        this->grid->addWidget(label_key_min_, j, 3);
        this->grid->addWidget(label_key_max_, j, 4);
    }
    
    styleLabels();
}

void MIDIChannelsSummary::clearData()
{
    QLayoutItem *item;
    while ((item = this->grid->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }
}

void MIDIChannelsSummary::styleLabels()
{
    for (int i=0; i < this->grid->count(); i++)
    {
        QLabel *label = static_cast<QLabel*>(this->grid->itemAt(i)->widget());
        QString stylesheet = "QLabel {"
                             "  font-size: 6pt;"
                             "}";
        label->setStyleSheet(stylesheet);
    }
}
