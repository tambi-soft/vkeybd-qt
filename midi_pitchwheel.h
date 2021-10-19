#ifndef MIDIPITCHWHEEL_H
#define MIDIPITCHWHEEL_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>

class MIDiPitchWheel : public QWidget
{
    Q_OBJECT
public:
    explicit MIDiPitchWheel(QWidget *parent = nullptr);
    
signals:
    
};

#endif // MIDIPITCHWHEEL_H
