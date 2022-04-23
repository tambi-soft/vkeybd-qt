#ifndef MIDIKEYSELECTOR_H
#define MIDIKEYSELECTOR_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>

class MIDIKeySelector : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIKeySelector(QWidget *parent = nullptr);
    
    QList<int> getListOfSelectedKeys();
    QString getBitmaskOfKeys();
    void restoreBitmaskOfKeys(QString bitmask);
    
private:
    QList<int> labels;
    QList<QCheckBox*> list_of_checkboxes;
    
signals:
    
};

#endif // MIDIKEYSELECTOR_H
