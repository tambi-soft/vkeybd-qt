#ifndef MIDIKEYSHIFTWIDGET_H
#define MIDIKEYSHIFTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

class MIDIKeyShiftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIKeyShiftWidget(QWidget *parent = nullptr);
    
    int valueStart();
    void setValueStart(int value);
    
    int value();
    void setValue(int value);
    void setValueFromGlobal(int value);
    
    void valueChanged(int value);
    
private:
    QSpinBox *spin_key;
    int value_start = 0;
    
signals:
    void signalValueChanged(int value);
    
public slots:
    void lowerShiftKeyPressed();
    void higherShiftKeyPressed();
};

#endif // MIDIKEYSHIFTWIDGET_H
