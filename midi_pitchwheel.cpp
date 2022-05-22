#include "midi_pitchwheel.h"

MIDIPitchWheel::MIDIPitchWheel(QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    grid->setContentsMargins(0, 0, 0, 0);
    
    QLabel *label_tether = new QLabel("Tether Strength");
    QLabel *label_pitch = new QLabel("Pitch");
    QLabel *label_vibamp = new QLabel("Vibrato Amplitude");
    
    this->slider_tether = new QSlider(Qt::Horizontal, this);
    this->slider_pitch = new QSlider(Qt::Horizontal, this);
    this->slider_vibamp = new QSlider(Qt::Horizontal, this);
    
    this->slider_tether->setRange(0, 254);
    this->slider_tether->setTickInterval(64);
    this->slider_tether->setTickPosition(QSlider::TicksBelow);
    this->slider_tether->setValue(64);
    
    // see the midi-protocol, e.g:
    // https://sites.uci.edu/camp2014/2014/04/30/managing-midi-pitchbend-messages/
    this->slider_pitch->setRange(0, 16383);
    this->slider_pitch->setTickInterval(8192);
    this->slider_pitch->setTickPosition(QSlider::TicksBelow);
    this->slider_pitch->setValue(8192);
    this->slider_pitch->setObjectName("slider_pitch");
    
    this->slider_vibamp->setRange(8192, 16383);
    //this->slider_vibamp->setRange(0, 8192);
    
    grid->addWidget(label_tether, 0, 0);
    grid->addWidget(this->slider_tether, 1, 0);
    
    grid->addWidget(label_pitch, 2, 0);
    grid->addWidget(this->slider_pitch, 3, 0, 1, 2);
    
    grid->addWidget(label_vibamp, 0, 1);
    grid->addWidget(this->slider_vibamp, 1, 1);
    
    this->slider_tether->setTracking(false);
    connect(this->slider_tether, &QSlider::valueChanged, this, &MIDIPitchWheel::startPitchThread);
    
    this->slider_pitch->setTracking(false);
    //connect(this->slider_pitch, &QSlider::valueChanged, this, &MIDIPitchWheel::startPitchThread);
    connect(this->slider_pitch, &QSlider::sliderMoved, this, &MIDIPitchWheel::sliderMoved);
    
    this->slider_vibamp->setTracking(false);
    //connect(this->slider_vibamp, &QSlider::valueChanged, this, &MIDIPitchWheel::vibrSliderMoved);
    connect(this->slider_vibamp, &QSlider::sliderMoved, this, &MIDIPitchWheel::vibrSliderMoved);
    
    this->worker = new MIDIPitchWheelWorker();
    connect(this->worker, &MIDIPitchWheelWorker::movePitchSlider, this, &MIDIPitchWheel::movePitchSlider);
    
    this->thread = new QThread(this);
    this->worker->moveToThread(this->thread);
    this->thread->start();
}

MIDIPitchWheel::~MIDIPitchWheel()
{
    this->thread->exit();
}

void MIDIPitchWheel::startPitchThread()
{
    int tether = this->slider_tether->value();
    int pitch = this->slider_pitch->value();
    int amp = this->slider_vibamp->value();
    
    this->worker->setTether(tether);
    this->worker->setPitch(pitch);
    this->worker->setVibAmp(amp);
}

void MIDIPitchWheel::movePitchSlider(int position)
{
    this->slider_pitch->setValue(position);
    
    emit pitchWheelMoved(position);
}

void MIDIPitchWheel::sliderMoved(int position)
{
    emit pitchWheelMoved(position);
}

void MIDIPitchWheel::movePitchWheel(int key)
{
    int value = 0;
    
    if (key == Qt::Key_Left)
    {
        value = 4096;
    }
    else if (key == Qt::Key_Right)
    {
        value = 12288;
    }
    else if (key == Qt::Key_Down)
    {
        value = 0;
    }
    else if (key == Qt::Key_Up)
    {
        value = 16384;
    }
    
    this->slider_pitch->setValue(value);
    //startPitchThread();
}
void MIDIPitchWheel::vibrSliderMoved(int position)
{
    //this->slider_pitch->blockSignals(true);
    
    pitchKeyPressed(KeysRaw::Right);
    this->worker->setVibAmp(position);
}
void MIDIPitchWheel::pitchKeyPressed(int key)
{
    int tether = this->slider_tether->value();
    int pitch = this->slider_pitch->value();
    int amp = this->slider_vibamp->value();
    
    this->worker->setTether(tether);
    this->worker->setPitch(pitch);
    this->worker->setVibAmp(amp);
    
    int direction = 0;
    if (key == KeysRaw::Left)
    {
        direction = -1;
    }
    else if (key == KeysRaw::Right)
    {
        direction = 1;
    }
    this->slider_pitch->blockSignals(true);
    this->worker->keyDown(direction);
}
void MIDIPitchWheel::pitchKeyReleased()
{
    this->slider_pitch->blockSignals(false);
    this->worker->keyUp();
}

QMap<QString,int> MIDIPitchWheel::getData()
{
    QMap<QString,int> result;
    
    result["pitchwheel_tether"] = this->slider_tether->value();
    result["pitchwheel_vibamp"] = this->slider_vibamp->value();
    result["pitchwheel_pitch"] = this->slider_pitch->value();
    
    return result;
}
void MIDIPitchWheel::setData(QMap<QString,QVariant> data)
{
    this->slider_tether->setValue(data["pitchwheel_tether"].toInt());
    this->slider_vibamp->setValue(data["pitchwheel_vibamp"].toInt());
    this->slider_pitch->setValue(data["pitchwheel_pitch"].toInt());
}





MIDIPitchWheelWorker::MIDIPitchWheelWorker(QObject *parent) : QObject(parent)
{
    this->timer = new QTimer(this);
    this->timer->setInterval(3);
    this->timer->setTimerType(Qt::PreciseTimer);
    connect(this->timer, &QTimer::timeout, this, &MIDIPitchWheelWorker::tick, Qt::DirectConnection);
    
    this->timer->start();
}

void MIDIPitchWheelWorker::setTether(int tether)
{
    this->tether = tether;
}
void MIDIPitchWheelWorker::setPitch(int pitch)
{
    this->pitch = pitch;
    
    if (this->pitch < 8192)
    {
        this->sign_positive = false;
    }
    else
    {
        this->sign_positive = true;
    }
}
void MIDIPitchWheelWorker::setVibAmp(int amp)
{
    this->amp = amp;
}

void MIDIPitchWheelWorker::keyDown(int direction)
{
    this->key_pressed = true;
    
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
void MIDIPitchWheelWorker::keyUp()
{
    this->key_pressed = false;
    
    this->direction = 0;
}

void MIDIPitchWheelWorker::tick()
{
    // controlling vibrato
    if (this->amp == 8192)
    {
        this->amp_cooldown = true;
        
        this->pitch = 8192;
        
    }
    else
    {
        if ((this->pitch < this->amp) && !this->amp_cooldown)
        {
            this->direction = 1;
        }
        else if (this->pitch >= this->amp)
        {
            this->direction = 0;
            this->amp_cooldown = true;
        }
        else if (this->pitch == 8192)
        {
            this->amp_cooldown = false;
        }
    }
    
    
    
    if (this->direction == 0)
    {
        // reset pitch wheel
        if (this->pitch != 8192)
        {
            if (this->pitch < 8192)
            {
                this->pitch = this->pitch + this->tether;
            }
            else if (this->pitch > 8192)
            {
                this->pitch = this->pitch - this->tether;
            }
            
            // if we shoot over 8192 we set pitch to 8192 and quit
            if ((this->sign_positive && this->pitch < 8192) || (!this->sign_positive && this->pitch > 8192))
            {
                this->pitch = 8192;
                
                //this->timer->setInterval(100);
            }
            
            emit movePitchSlider(this->pitch);
        }
    }
    else
    {
        // pitch wheel moved as long as key pressed
        this->pitch = this->pitch + this->direction * this->tether;
        
        // forbid over/undershooting valid range
        if (this->pitch < 0)
            this->pitch = 0;
        else if (this->pitch > 16383)
            this->pitch = 16383;
        
        emit movePitchSlider(this->pitch);
    }
}
