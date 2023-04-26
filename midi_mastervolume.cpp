#include "midi_mastervolume.h"

MIDIMasterVolume::MIDIMasterVolume(QObject *parent)
    : QObject{parent}
{
    //QVBoxLayout *layout = new QVBoxLayout;
    //setLayout(layout);
    //layout->setMargin(0);
    //layout->setContentsMargins(0, 0, 0, 0);
    
    this->label_volume = new QLabel("Master Volume (DCA): 100%");
    this->slider_volume = new QSlider(Qt::Horizontal);
    
    this->label_tether = new QLabel("Volume Tether Strength");
    this->slider_tether = new QSlider(Qt::Horizontal);
    
    this->slider_tether->setRange(0, 150);
    this->slider_tether->setTickInterval(20);
    this->slider_tether->setTickPosition(QSlider::TicksBelow);
    this->slider_tether->setValue(20);
    
    this->slider_volume->setRange(0, 120);
    this->slider_volume->setTickInterval(20);
    this->slider_volume->setTickPosition(QSlider::TicksBelow);
    this->slider_volume->setValue(100);
    
    connect(this->slider_tether, &QSlider::valueChanged, this, &MIDIMasterVolume::startThread);
    
    connect(this->slider_volume, &QSlider::valueChanged, this, &MIDIMasterVolume::volumeSliderMoved);
    
    this->thread = new QThread(this);
    
    this->worker = new MIDIMasterVolumeWorker();
    connect(this->thread, &QThread::started, this->worker, &MIDIMasterVolumeWorker::initTimer);
    connect(this->worker, &MIDIMasterVolumeWorker::timerNeedToStart, this->worker, &MIDIMasterVolumeWorker::startTimer);
    connect(this->worker, &MIDIMasterVolumeWorker::moveVolumeSlider, this, &MIDIMasterVolume::moveVolumeSlider);
    
    this->worker->moveToThread(this->thread);
    this->thread->start();
}

MIDIMasterVolume::~MIDIMasterVolume()
{
    this->thread->exit();
}

void MIDIMasterVolume::startThread()
{
    int tether = this->slider_tether->value();
    int pitch = this->slider_volume->value();
    
    this->worker->setTether(tether);
    this->worker->setVolume(pitch);
}

void MIDIMasterVolume::moveVolumeSlider(int value)
{
    this->slider_volume->setValue(value);
    
    emit sliderMoved(value);
    
    updateSliderLabel(value);
}

void MIDIMasterVolume::volumeSliderMoved(int value)
{
    emit sliderMoved(value);
    
    updateSliderLabel(value);
}
void MIDIMasterVolume::updateSliderLabel(int value)
{
    QString label = this->label_volume->text().split(":").at(0);
    label += ": " + QString::number(value) + "%";
    
    this->label_volume->setText(label);
}

void MIDIMasterVolume::volumeKeyPressed(int key)
{
    int tether = this->slider_tether->value();
    int volume = this->slider_volume->value();
    
    this->worker->setTether(tether);
    this->worker->setVolume(volume);
    this->worker->setVolumeMinMax(this->slider_volume->minimum(), this->slider_volume->maximum());
    
    int direction = 0;
    if (key == KeysRaw::Down)
    {
        direction = -1;
        this->worker->shouldResetSlider(true);
    }
    else if (key == KeysRaw::Up)
    {
        direction = 1;
        this->worker->shouldResetSlider(true);
    }
    else if (key == KeysRaw::PageDown)
    {
        direction = -1;
        this->worker->shouldResetSlider(false);
    }
    else if (key == KeysRaw::PageUp)
    {
        direction = 1;
        this->worker->shouldResetSlider(false);
    }
    
    this->slider_volume->blockSignals(true);
    this->worker->keyDown(direction);
}
void MIDIMasterVolume::volumeKeyReleased()
{
    this->slider_volume->blockSignals(false);
    this->worker->keyUp();
}

int MIDIMasterVolume::getValue()
{
    return this->slider_volume->value();
}
void MIDIMasterVolume::setValue(int value)
{
    this->slider_volume->setValue(value);
}





MIDIMasterVolumeWorker::MIDIMasterVolumeWorker(QObject *parent)
    : QObject{parent}
{
    /*
    this->timer = new QTimer(this);
    this->timer->setInterval(5);
    this->timer->setTimerType(Qt::PreciseTimer);
    connect(this->timer, &QTimer::timeout, this, &MIDIMasterVolumeWorker::tick, Qt::DirectConnection);
    
    this->timer->start();
    */
}

void MIDIMasterVolumeWorker::setTether(int tether)
{
    this->tether = static_cast<float>(tether) / 200;
}
void MIDIMasterVolumeWorker::setVolume(int value)
{
    this->volume = value;
    
    if (this->volume < 100)
    {
        this->sign_positive = false;
    }
    else
    {
        this->sign_positive = true;
    }
}
void MIDIMasterVolumeWorker::setVolumeMinMax(int value_min, int value_max)
{
    this->volume_min = value_min;
    this->volume_max = value_max;
}

void MIDIMasterVolumeWorker::shouldResetSlider(bool reset)
{
    this->reset_slider = reset;
}

void MIDIMasterVolumeWorker::keyDown(int direction)
{
    emit timerNeedToStart();
    
    this->direction = direction;
    
    if (this->direction < 0)
    {
        this->sign_positive = false;
    }
    else if (this->direction > 0)
    {
        this->sign_positive = true;
    }
}
void MIDIMasterVolumeWorker::keyUp()
{
    this->direction = 0;
}
void MIDIMasterVolumeWorker::initTimer()
{
    this->timer = new QTimer(this);
    this->timer->setInterval(3);
    this->timer->setTimerType(Qt::PreciseTimer);
    connect(this->timer, &QTimer::timeout, this, &MIDIMasterVolumeWorker::tick, Qt::DirectConnection);
}
void MIDIMasterVolumeWorker::startTimer()
{
    this->timer->start();
}

void MIDIMasterVolumeWorker::tick()
{
    if (this->direction == 0)
    {
        // reset volume slider
        if (this->volume != 100 && this->reset_slider)
        {
            if (this->volume < 100)
            {
                this->volume = this->volume + this->tether;
            }
            else if (this->volume > 100)
            {
                this->volume = this->volume - this->tether;
            }
            
            // if we shoot over 100 we set volume to 100 and quit
            if ((this->sign_positive && this->volume < 100) || (!this->sign_positive && this->volume > 100))
            {
                this->volume = 100;
                
                this->timer->stop();
            }
            
            emit moveVolumeSlider(this->volume);
        }
        else
        {
            this->timer->stop();
        }
    }
    else
    {
        // pitch wheel moved as long as key pressed
        if (this->volume >= this->volume_min && this->volume <= this->volume_max)
        {
            this->volume = this->volume + this->direction * this->tether;
        }
        
        emit moveVolumeSlider(this->volume);
    }
}
