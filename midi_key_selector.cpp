#include "midi_key_selector.h"

MIDIKeySelector::MIDIKeySelector(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);
    
    layout->setContentsMargins(0, 0, 0, 0);
    
    this->labels << "0" << "12" << "24" << "36" << "48" << "60" << "72" << "84";
    
    for (int i=0; i<8; i++)
    {
        QCheckBox *check = new QCheckBox(labels.at(i));
        
        layout->addWidget(check);
        
        if (i == 4)
        {
            check->setChecked(true);
        }
    }
}
