#include "midi_channels_summary.h"

MIDIChannelsSummary::MIDIChannelsSummary(QWidget *parent) : QWidget(parent)
{
    this->grid = new QGridLayout;
    setLayout(this->grid);
}

void MIDIChannelsSummary::showData(QList<QMap<QString, QVariant>> data)
{
    clearData();
    
    QLabel *label_channel_number = new QLabel("<b>Channel</b>");
    QLabel *label_volume = new QLabel("<b>Volume</b>");
    QLabel *label_key_shift = new QLabel("<b>Key Shift</b>");
    QLabel *label_key_min = new QLabel("<b>Key Min</b>");
    QLabel *label_key_max = new QLabel("<b>Key Max</b>");
    //QLabel *label_msb = new QLabel("<b>MSB</b>");
    //QLabel *label_lsb = new QLabel("<b>LSB</b>");
    QLabel *label_instrument = new QLabel("<b>Instrument</b>");
    
    this->grid->addWidget(label_channel_number, 0, 0);
    this->grid->addWidget(label_volume, 0, 1);
    this->grid->addWidget(label_key_shift, 0, 2);
    this->grid->addWidget(label_key_min, 0, 3);
    this->grid->addWidget(label_key_max, 0, 4);
    //this->grid->addWidget(label_msb, 0, 5);
    //this->grid->addWidget(label_lsb, 0, 6);
    this->grid->addWidget(label_instrument, 0, 7);
    
    for (int i=0; i < data.length(); i++)
    {
        QLabel *label_channel_ = new QLabel(QString::number(data.at(i)["channel"].toInt()+1));
        QLabel *label_volume_ = new QLabel(QString::number(data.at(i)["volume"].toInt()));
        QLabel *label_key_shift_ = new QLabel(QString::number(data.at(i)["key_shift"].toInt()));
        QLabel *label_key_min_ = new QLabel(QString::number(data.at(i)["key_min"].toInt()));
        QLabel *label_key_max_ = new QLabel(QString::number(data.at(i)["key_max"].toInt()));
        //QLabel *label_msb_ = new QLabel(QString::number(data.at(i)["instrument_msb"].toInt()));
        //QLabel *label_lsb_ = new QLabel(QString::number(data.at(i)["instrument_lsb"].toInt()));
        QLabel *label_instrument_ = new QLabel(data.at(i)["instrument_name"].toString());
        
        int j = i+1;
        
        this->grid->addWidget(label_channel_, j, 0);
        this->grid->addWidget(label_volume_, j, 1);
        this->grid->addWidget(label_key_shift_, j, 2);
        this->grid->addWidget(label_key_min_, j, 3);
        this->grid->addWidget(label_key_max_, j, 4);
        //this->grid->addWidget(label_msb_, j, 5);
        //this->grid->addWidget(label_lsb_, j, 6);
        this->grid->addWidget(label_instrument_, j, 7);
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
