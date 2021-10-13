#ifndef MIDIKEYSELECTOR_H
#define MIDIKEYSELECTOR_H

#include <QObject>
#include <QWidget>

#include <QHBoxLayout>
#include <QCheckBox>

class MIDIKeySelector : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIKeySelector(QWidget *parent = nullptr);
    
private:
    QList<QString> labels;
    QList<QCheckBox*> list_of_checkboxes;
    
signals:
    
};

#endif // MIDIKEYSELECTOR_H
