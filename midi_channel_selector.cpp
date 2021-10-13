#include "midi_channel_selector.h"

MIDIChannelSelector::MIDIChannelSelector(QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    
    grid->setContentsMargins(0, 0, 0, 0);
    
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
}
