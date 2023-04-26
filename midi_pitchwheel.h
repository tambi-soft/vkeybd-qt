#ifndef MIDIPITCHWHEEL_H
#define MIDIPITCHWHEEL_H

#include <QThread>
#include <QTimer>

#include <interface_audio.h>

#include "enums_structs.h"

class MIDIPitchWheelWorker : public QObject
{
    Q_OBJECT
    
public:
    explicit MIDIPitchWheelWorker(QObject *parent = 0);
    
    void setTether(int tether);
    void setPitch(int pitch);
    void shouldResetSlider(bool reset);
    void keyDown(int direction); // direction is either -1, 0, 1
    void keyUp();
    
private:
    QTimer *timer;
    
    int tether;
    int pitch = 8192;
    bool amp_cooldown = false;
    int direction = 0; // direction is either -1, 0, 1
    
    bool sign_positive; // false: -, true: +
    
    bool reset_slider = true;
    
//protected:
public slots:
    void tick();
    void initTimer();
    void startTimer();
    
signals:
    void movePitchSlider(int step);
    void timerNeedToStart();
};



#include <QObject>
#include <QSlider>
#include <QLabel>
#include <QDebug>

class MIDIPitchWheel : public QObject
{
    Q_OBJECT
public:
    explicit MIDIPitchWheel(QObject *parent = nullptr);
    ~MIDIPitchWheel();
    
    QLabel *label_tether;
    QLabel *label_pitch;
    
    QSlider *slider_tether;
    QSlider *slider_pitch;
    
    void movePitchWheel(int key);
    void pitchKeyPressed(int key);
    void pitchKeyReleased();
    
    QMap<QString,int> getData();
    void setData(QMap<QString,QVariant> data);
    
private:
    QThread *thread;
    MIDIPitchWheelWorker *worker;
    
    void startPitchThread();
    void movePitchSlider(int position);
    void sliderMoved(int position);
    void updateLabel(int position);
    
signals:
    void pitchWheelMoved(int position);
};

#endif // MIDIPITCHWHEEL_H
