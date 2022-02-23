#include "midi_mastervolume.h"

MIDIMastervolume::MIDIMastervolume(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    layout->setMargin(0);
    
    this->label_volume_master = new QLabel("Master Volume (DCA): 100%");
    this->slider_volume_master = new QSlider(Qt::Horizontal, this);
    this->slider_volume_master->setRange(0, 120);
    this->slider_volume_master->setTickInterval(20);
    this->slider_volume_master->setTickPosition(QSlider::TicksBelow);
    this->slider_volume_master->setValue(100);
    
    connect(this->slider_volume_master, &QSlider::valueChanged, this, &MIDIMastervolume::volumeSliderMoved);
    
    layout->addWidget(this->label_volume_master);
    layout->addWidget(this->slider_volume_master);
}

void MIDIMastervolume::volumeSliderMoved(int value)
{
    emit sliderMoved(value);
    
    QString label = this->label_volume_master->text().split(":").at(0);
    label += ": " + QString::number(value) + "%";
    
    this->label_volume_master->setText(label);
}

void MIDIMastervolume::volumeKeyPressed(int key)
{
    int direction = 0;
    if (key == Qt::Key_Left)
    {
        direction = -1;
    }
    else if (key == Qt::Key_Right)
    {
        direction = 1;
    }
}
void MIDIMastervolume::volumeKeyReleased(int key)
{
    
}




MIDIMastervolumeWorker::MIDIMastervolumeWorker(QObject *parent)
    : QObject{parent}
{
    
}

void MIDIMastervolumeWorker::keyDown(int direction)
{
    
}
void MIDIMastervolumeWorker::keyUp()
{
    
}

void MIDIMastervolumeWorker::tick()
{
    
}
