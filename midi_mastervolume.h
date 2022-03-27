#ifndef MIDI_MASTERVOLUME_H
#define MIDI_MASTERVOLUME_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QDebug>

class MIDIMasterVolumeWorker : public QObject
{
    Q_OBJECT
    
public:
    explicit MIDIMasterVolumeWorker(QObject *parent = 0);
    
    void setVolume(int value);
    void setVolumeMinMax(int value_min, int value_max);
    void shouldResetSlider(bool reset);
    void keyDown(int direction); // direction is either -1, 0, 1
    void keyUp();
    
private:
    QTimer *timer;
    
    qreal volume = 100;
    int volume_min = 0;
    int volume_max = 120;
    qreal tether = .1;
    bool reset_slider = true;
    
    int direction = 0; // direction is either -1, 0, 1
    
    bool sign_positive; // false: -, true: +
    
//protected:
public slots:
    void tick();
    
signals:
    void moveVolumeSlider(int step);
    
};





#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>

class MIDIMasterVolume : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIMasterVolume(QWidget *parent = nullptr);
    ~MIDIMasterVolume();
    
    void volumeKeyPressed(int key);
    void volumeKeyReleased();
    
private:
    QLabel *label_volume;
    QSlider *slider_volume;
    
    QThread *thread;
    MIDIMasterVolumeWorker *worker;
    
    void moveVolumeSlider(int value);
    void updateSliderLabel(int value);
    
private slots:
    void volumeSliderMoved(int value);
    
signals:
    void sliderMoved(int value);
    
};

#endif // MIDI_MASTERVOLUME_H