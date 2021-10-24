#ifndef MIDICHANNELSELECTOR_H
#define MIDICHANNELSELECTOR_H

#include <QObject>
#include <QWidget>

#include <QGridLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QDial>
#include <QComboBox>
#include <QLabel>

#include "midi_sounds_list.h"

class MIDIChannelSelector : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIChannelSelector(QWidget *parent = nullptr);
    
private:
    QList<QCheckBox*> list_of_checkboxes;
    
signals:
    
};



#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>

class MIDIKeyShiftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIKeyShiftWidget(QWidget *parent = nullptr);
    
private:
    QSpinBox *spin_key;
    
signals:
    
private slots:
    void lowerShiftKeyPressed();
    void higherShiftKeyPressed();
};

#endif // MIDICHANNELSELECTOR_H
