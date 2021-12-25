#include "midi_key_selector.h"

MIDIKeySelector::MIDIKeySelector(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout_vertical = new QVBoxLayout;
    QHBoxLayout *layout_keys = new QHBoxLayout;
    QHBoxLayout *layout_shift = new QHBoxLayout;
    //setLayout(layout_horizontal);
    setLayout(layout_vertical);
    
    layout_vertical->setContentsMargins(0, 0, 0, 0);
    layout_keys->setContentsMargins(0, 0, 0, 0);
    layout_shift->setContentsMargins(0, 0, 0, 0);
    
    this->labels = {0, 12, 24, 36, 48, 60, 72, 84};
    
    for (int i=0; i<8; i++)
    {
        QCheckBox *check = new QCheckBox(QString::number(labels.at(i)));
        
        layout_keys->addWidget(check);
        
        if (i == 4)
        {
            check->setChecked(true);
        }
        
        QString stylesheet = "QCheckBox:indicator:checked {"
                             "  color: white;"
                             "  background-color: black;"
                             "}";
        
        check->setStyleSheet(stylesheet);
        
        this->list_of_checkboxes.append(check);
    }
    
    layout_vertical->addLayout(layout_keys);
    
    //QLabel *label_key_shift = new QLabel("Key Shift:");
    
    //layout_shift->addStretch();
    //layout_shift->addStretch();
    
    //layout_vertical->addLayout(layout_shift);
    
}

QList<int> MIDIKeySelector::getListOfSelectedKeys()
{
    QList<int> result;
    for (int i=0; i < this->list_of_checkboxes.length(); i++)
    {
        QCheckBox *check = this->list_of_checkboxes.at(i);
        if (check->isChecked())
        {
            result.append(this->labels.at(i));
        }
    }
    return result;
}
