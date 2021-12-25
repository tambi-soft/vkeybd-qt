#ifndef MIDIKEYSELECTOR_H
#define MIDIKEYSELECTOR_H

#include <QObject>
#include <QWidget>

#include <QHBoxLayout>
#include <QCheckBox>

#include "midi_key_shift_widget.h"

class MIDIKeySelector : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIKeySelector(QWidget *parent = nullptr);
    
    QList<int> getListOfSelectedKeys();
    
private:
    QList<int> labels;
    QList<QCheckBox*> list_of_checkboxes;
    
signals:
    
};

#endif // MIDIKEYSELECTOR_H
