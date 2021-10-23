#include "midi_channel_selector.h"

MIDIChannelSelector::MIDIChannelSelector(QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    
    //grid->setContentsMargins(0, 0, 0, 0);
    
    QLabel *channel_label = new QLabel("Channel");
    QLabel *key_min_label = new QLabel("Key Min");
    QLabel *key_max_label = new QLabel("Key Max");
    QLabel *key_shift_label = new QLabel("Key Shift");
    
    grid->addWidget(channel_label, 0, 0);
    grid->addWidget(key_min_label, 0, 2);
    grid->addWidget(key_max_label, 0, 4);
    grid->addWidget(key_shift_label, 0, 5);
    
    for (int i=1; i<=16; i++)
    {
        QCheckBox *check = new QCheckBox(QString::number(i));
        QSpinBox *key_min = new QSpinBox();
        QSpinBox *key_max = new QSpinBox();
        QSpinBox *key_shift = new QSpinBox();
        
        key_min->setMaximum(127);
        key_max->setMaximum(127);
        
        key_min->setValue(0);
        key_max->setValue(127);
        
        QComboBox *combo_categories = new QComboBox;
        QStringList list_categories = MIDISoundsList::getCategories();
        combo_categories->addItems(list_categories);
        
        grid->addWidget(check, i, 0);
        //grid->addWidget(key_min_label, i, 1);
        grid->addWidget(key_min, i, 2);
        //grid->addWidget(key_max_label, i, 3);
        grid->addWidget(key_max, i, 4);
        grid->addWidget(combo_categories, i, 5);
        
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
