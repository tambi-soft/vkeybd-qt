#include "midi_pitchwheel.h"

MIDIPitchWheel::MIDIPitchWheel(QObject *parent) : QObject(parent)
{
    this->label_tether = new QLabel("Pitch Tether Strength");
    this->label_pitch = new QLabel("Pitch");
    
    this->slider_pitch = new QSlider(Qt::Horizontal);
    this->slider_tether = new QSlider(Qt::Horizontal);
    
    this->slider_tether->setRange(0, 254);
    this->slider_tether->setTickInterval(64);
    this->slider_tether->setTickPosition(QSlider::TicksBelow);
    this->slider_tether->setValue(64);
    
    // see the midi-protocol, e.g:
    // https://sites.uci.edu/camp2014/2014/04/30/managing-midi-pitchbend-messages/
    this->slider_pitch->setRange(0, 16383);
    this->slider_pitch->setTickInterval(4096);
    this->slider_pitch->setTickPosition(QSlider::TicksBelow);
    this->slider_pitch->setValue(8192);
    this->slider_pitch->setObjectName("slider_pitch");
    
    this->slider_tether->setTracking(false);
    connect(this->slider_tether, &QSlider::valueChanged, this, &MIDIPitchWheel::startPitchThread);
    
    this->slider_pitch->setTracking(false);
    connect(this->slider_pitch, &QSlider::sliderMoved, this, &MIDIPitchWheel::sliderMoved);
    
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
    
    this->worker->setTether(tether);
    this->worker->setPitch(pitch);
}

void MIDIPitchWheel::movePitchSlider(int position)
{
    this->slider_pitch->setValue(position);
    
    updateLabel(position);
    
    emit pitchWheelMoved(position);
}
void MIDIPitchWheel::updateLabel(int position)
{
    /*
    Standard MIDI uses a pitch wheel range of +/-2 semitones = 200 cents. MIDI pitch bend wheel resolution (according to the spec) is +8192/-8191. This means there are 8192/200 = 40.96 pitch bend units to 1 cent.
    
    http://www.elvenminstrel.com/music/tuning/reference/pitchbends.shtml
    */
    float CENT_FACTOR = 40.96;
    float cent = position / CENT_FACTOR - 200; // -200 to center at 0
    QString indicator = "";
    if (position == 8192)
        indicator = "";
    else if (position < 8192 && position >= 4096)
        indicator = "<<";
    else if (position > 8192 && position <= 8192 + 4096)
        indicator = ">>";
    else if (position < 4096)
        indicator = "<<<<";
    else if (position > 8192 + 4096)
        indicator = ">>>>";
    
    this->label_pitch->setText("Pitch: "+indicator+" "+QString::number(cent)+" Cents");// / "+QString::number(position-8192));
}

void MIDIPitchWheel::sliderMoved(int position)
{
    updateLabel(position);
    
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
    if (key == KeysRaw::Left)
    {
        direction = -1;
        this->worker->shouldResetSlider(true);
    }
    else if (key == KeysRaw::Right)
    {
        direction = 1;
        this->worker->shouldResetSlider(true);
    }
    else if (key == KeysRaw::End)
    {
        direction = -1;
        this->worker->shouldResetSlider(false);
    }
    else if (key == KeysRaw::Home)
    {
        direction = 1;
        this->worker->shouldResetSlider(false);
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
    result["pitchwheel_pitch"] = this->slider_pitch->value();
    
    return result;
}
void MIDIPitchWheel::setData(QMap<QString,QVariant> data)
{
    this->slider_tether->setValue(data["pitchwheel_tether"].toInt());
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

void MIDIPitchWheelWorker::shouldResetSlider(bool reset)
{
    if (!reset)
        // we want to move the slider 8 times slower if using the home/end keys
        this->tether = this->tether / 8;
    
    this->reset_slider = reset;
}

void MIDIPitchWheelWorker::keyDown(int direction)
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
void MIDIPitchWheelWorker::keyUp()
{
    this->direction = 0;
}

void MIDIPitchWheelWorker::tick()
{
    if (this->direction == 0)
    {
        // reset pitch wheel
        if (this->pitch != 8192 && this->reset_slider)
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
