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
    
    int value();
    void setValue(int value);
    
private:
    QSpinBox *spin_key;
    
signals:
    void valueChanged(int value);
    
public slots:
    void lowerShiftKeyPressed();
    void higherShiftKeyPressed();
};

#endif // MIDIKEYSHIFTWIDGET_H
