#ifndef MIDIPITCHWHEEL_H
#define MIDIPITCHWHEEL_H

#include <QThread>
#include <QTimer>

class MIDIPitchWheelWorker : public QObject
{
    Q_OBJECT
    
public:
    explicit MIDIPitchWheelWorker(QObject *parent = 0);
    
    void setValues(int tether, int pitch);
    
private:
    QTimer *timer;
    
    int tether;
    int pitch = 8192;
    
    bool sign_positive; // false: -, true: +
    
//protected:
public slots:
    void tick();
    
signals:
    void movePitchSlider(int step);
    
};



#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <QDebug>

class MIDIPitchWheel : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIPitchWheel(QWidget *parent = nullptr);
    ~MIDIPitchWheel();
    
    void movePitchSlider(int position);
    
private:
    QSlider *slider_tether;
    QSlider *slider_pitch;
    
    QThread *thread;
    MIDIPitchWheelWorker *worker;
    
    void startPitchThread();
    
signals:
    
};

#endif // MIDIPITCHWHEEL_H
