#include "midi_channel_selector.h"

MIDIChannelSelector::MIDIChannelSelector(QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    
    //grid->setContentsMargins(0, 0, 0, 0);
    
    /**
    for (int i=0; i<16; i++)
    {
        QCheckBox *check = new QCheckBox(QString::number(i));
        
        int row = 0;
        int column = 0;
        if (i<8)
        {
            column = i;
        }
        else
        {
            row = 1;
            column = i-8;
        }
        grid->addWidget(check, row, column);
        this->list_of_checkboxes.append(check);
        
        if (i==0)
        {
            check->setChecked(true);
        }
    }
    **/
    
    QLabel *channel_label = new QLabel("Channel");
    QLabel *key_min_label = new QLabel("Key Min");
    QLabel *key_max_label = new QLabel("Key Max");
    
    grid->addWidget(channel_label, 0, 0);
    grid->addWidget(key_min_label, 0, 2);
    grid->addWidget(key_max_label, 0, 4);
    
    for (int i=1; i<=16; i++)
    {
        QCheckBox *check = new QCheckBox(QString::number(i));
        QSpinBox *key_min = new QSpinBox();
        QSpinBox *key_max = new QSpinBox();
        
        key_min->setMaximum(127);
        key_max->setMaximum(127);
        
        key_min->setValue(0);
        key_max->setValue(127);
        
        grid->addWidget(check, i, 0);
        //grid->addWidget(key_min_label, i, 1);
        grid->addWidget(key_min, i, 2);
        //grid->addWidget(key_max_label, i, 3);
        grid->addWidget(key_max, i, 4);
        
        if (i==1)
        {
            check->setChecked(true);
        }
        
        QString stylesheet = "QCheckBox:indicator:checked {"
                             "  color: white;"
                             "  background-color: black;"
                             "}";
        
        check->setStyleSheet(stylesheet);
    }
}
