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
    this->slider_volume->setRange(0, 120);
    this->slider_volume->setTickInterval(20);
    this->slider_volume->setTickPosition(QSlider::TicksBelow);
    this->slider_volume->setValue(100);
    
    this->label_tether = new QLabel("Volume Tether Strength");
    this->slider_tether = new QSlider(Qt::Horizontal);
    
    connect(this->slider_volume, &QSlider::valueChanged, this, &MIDIMasterVolume::volumeSliderMoved);
    
    //layout->addWidget(this->label_volume);
    //layout->addWidget(this->slider_volume);
    
    this->worker = new MIDIMasterVolumeWorker();
    connect(this->worker, &MIDIMasterVolumeWorker::moveVolumeSlider, this, &MIDIMasterVolume::moveVolumeSlider);
    
    this->thread = new QThread(this);
    this->worker->moveToThread(this->thread);
    this->thread->start();
}

MIDIMasterVolume::~MIDIMasterVolume()
{
    this->thread->exit();
}

void MIDIMasterVolume::moveVolumeSlider(int value)
{
    qDebug() << "MMMMMMMMMO";
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
    int volume = this->slider_volume->value();
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
    this->timer = new QTimer(this);
    this->timer->setInterval(5);
    this->timer->setTimerType(Qt::PreciseTimer);
    connect(this->timer, &QTimer::timeout, this, &MIDIMasterVolumeWorker::tick, Qt::DirectConnection);
    
    this->timer->start();
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

void MIDIMasterVolumeWorker::tick()
{
    if (this->direction == 0)
    {
        // reset volume slider
        if (this->volume != 100 && this->reset_slider)
        {
            qDebug() << "aaaaaaaa";
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
                
                //this->timer->setInterval(100);
            }
            
            emit moveVolumeSlider(this->volume);
        }
    }
    else
    {
        qDebug() << "bbbbbbbb";
        // pitch wheel moved as long as key pressed
        if (this->volume >= this->volume_min && this->volume <= this->volume_max)
        {
            this->volume = this->volume + this->direction * this->tether;
        }
        
        emit moveVolumeSlider(this->volume);
    }
}
