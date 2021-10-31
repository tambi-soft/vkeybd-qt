#ifndef MIDIPITCHWHEEL_H
#define MIDIPITCHWHEEL_H

#include <QThread>
#include <QTimer>

#include <interface_audio.h>

class MIDIPitchWheelResetWorker : public QObject
{
    Q_OBJECT
    
public:
    explicit MIDIPitchWheelResetWorker(QObject *parent = 0);
    
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
    
    void movePitchWheel(int key);
    
private:
    QSlider *slider_tether;
    QSlider *slider_pitch;
    
    QThread *thread;
    MIDIPitchWheelResetWorker *worker;
    
    void startPitchThread();
    void movePitchSlider(int position);
    void sliderMoved(int position);
    
signals:
    void pitchWheelMoved(int position);
};

#endif // MIDIPITCHWHEEL_H
