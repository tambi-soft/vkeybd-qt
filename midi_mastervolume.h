#ifndef MIDI_MASTERVOLUME_H
#define MIDI_MASTERVOLUME_H

#include <QObject>
#include <QThread>
#include <QTimer>

class MIDIMastervolumeWorker : public QObject
{
    Q_OBJECT
    
public:
    explicit MIDIMastervolumeWorker(QObject *parent = 0);
    
    void keyDown(int direction); // direction is either -1, 0, 1
    void keyUp();
    
private:
    QTimer *timer;
    
    int volume = 100;
    
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

class MIDIMastervolume : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIMastervolume(QWidget *parent = nullptr);
    
    void volumeKeyPressed(int key);
    void volumeKeyReleased(int key);
    
private:
    QLabel *label_volume_master;
    QSlider *slider_volume_master;
    
private slots:
    void volumeSliderMoved(int value);
    
signals:
    void sliderMoved(int value);
    
};

#endif // MIDI_MASTERVOLUME_H
