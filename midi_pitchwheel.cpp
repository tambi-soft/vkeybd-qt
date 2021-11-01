#include "midi_pitchwheel.h"

MIDIPitchWheel::MIDIPitchWheel(QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    grid->setContentsMargins(0, 0, 0, 0);
    
    QLabel *label_tether = new QLabel("tether strength");
    QLabel *label_pitch = new QLabel("pitch");
    
    this->slider_tether = new QSlider(Qt::Horizontal, this);
    this->slider_pitch = new QSlider(Qt::Horizontal, this);
    
    this->slider_tether->setRange(0, 127);
    this->slider_tether->setTickInterval(64);
    this->slider_tether->setTickPosition(QSlider::TicksBelow);
    this->slider_tether->setValue(64);
    
    // see the midi-protocol, e.g:
    // https://sites.uci.edu/camp2014/2014/04/30/managing-midi-pitchbend-messages/
    this->slider_pitch->setRange(0, 16383);
    this->slider_pitch->setTickInterval(8192);
    this->slider_pitch->setTickPosition(QSlider::TicksBelow);
    this->slider_pitch->setValue(8192);
    
    grid->addWidget(label_tether, 0, 0);
    grid->addWidget(label_pitch, 0, 1);
    grid->addWidget(this->slider_tether, 1, 0);
    grid->addWidget(this->slider_pitch, 1, 1);
    
    QString style = "QSlider::groove:horizontal {"
            "   border: 1px solid;"
            "   height: 10px;"
            "   margin: 0px;"
            "}"
            "QSlider::handle:horizontal {"
            "   background-color: black;"
            "   border: 1px solid;"
            "   height: 40px;"
            "   width: 20px;"
            "   margin: -15px 0px;"
            "}";
    // https://stackoverflow.com/questions/27531542/tick-marks-disappear-on-styled-qslider
    this->slider_tether->setStyleSheet(style);
    this->slider_pitch->setStyleSheet(style);
    
    this->slider_tether->setTracking(false);
    connect(this->slider_tether, &QSlider::valueChanged, this, &MIDIPitchWheel::startPitchThread);
    this->slider_pitch->setTracking(false);
    connect(this->slider_pitch, &QSlider::valueChanged, this, &MIDIPitchWheel::startPitchThread);
    connect(this->slider_pitch, &QSlider::sliderMoved, this, &MIDIPitchWheel::sliderMoved);
    
    this->worker = new MIDIPitchWheelResetWorker();
    connect(this->worker, &MIDIPitchWheelResetWorker::movePitchSlider, this, &MIDIPitchWheel::movePitchSlider);
    
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
    
    this->worker->setTether(tether);
    this->worker->setPitch(pitch);
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
void MIDIPitchWheel::pitchKeyPressed(int key)
{
    int tether = this->slider_tether->value();
    int pitch = this->slider_pitch->value();
    
    this->worker->setTether(tether);
    this->worker->setPitch(pitch);
    
    int direction = 0;
    if (key == Qt::Key_Left)
    {
        direction = -1;
    }
    else if (key == Qt::Key_Right)
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



MIDIPitchWheelResetWorker::MIDIPitchWheelResetWorker(QObject *parent) : QObject(parent)
{
    this->timer = new QTimer(this);
    this->timer->setInterval(3);
    this->timer->setTimerType(Qt::PreciseTimer);
    connect(this->timer, &QTimer::timeout, this, &MIDIPitchWheelResetWorker::tick, Qt::DirectConnection);
    
    this->timer->start();
}

void MIDIPitchWheelResetWorker::setTether(int tether)
{
    this->tether = tether;
}
void MIDIPitchWheelResetWorker::setPitch(int pitch)
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

void MIDIPitchWheelResetWorker::keyDown(int direction)
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
void MIDIPitchWheelResetWorker::keyUp()
{
    this->direction = 0;
}

void MIDIPitchWheelResetWorker::tick()
{
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
            }
            
            emit movePitchSlider(this->pitch);
        }
    }
    else
    {
        // pitch wheel moved as long as key pressed
        this->pitch = this->pitch + this->direction * this->tether;
        
        emit movePitchSlider(this->pitch);
    }
}
