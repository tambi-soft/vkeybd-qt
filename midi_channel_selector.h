#ifndef MIDICHANNELSELECTOR_H
#define MIDICHANNELSELECTOR_H

#include <QObject>
#include <QWidget>

#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>

#include <QDebug>

class MIDIKeyShiftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIKeyShiftWidget(QWidget *parent = nullptr);
    
    QSpinBox *spin_key;
    
private:
    
signals:
    
private slots:
    void lowerShiftKeyPressed();
    void higherShiftKeyPressed();
};




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
    
    QList<QMap<QString, int> > getListOfActivatedChannels();
    
private:
    QList<QCheckBox*> list_of_checkboxes;
    QList<MIDIKeyShiftWidget*> list_of_keyshifts;
    QList<QSpinBox*> list_of_key_mins;
    QList<QSpinBox*> list_of_key_maxs;
    
signals:
    
};

#endif // MIDICHANNELSELECTOR_H
