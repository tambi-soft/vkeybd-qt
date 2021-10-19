#include "midi_pitchwheel.h"

MIDiPitchWheel::MIDiPitchWheel(QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    
    QLabel *label_tether = new QLabel("tether strength");
    QLabel *label_pitch = new QLabel("pitch");
    
    QSlider *slider_tether = new QSlider(Qt::Horizontal, this);
    QSlider *slider_pitch = new QSlider(Qt::Horizontal, this);
    
    slider_tether->setRange(0, 127);
    slider_tether->setValue(0);
    
    // see the midi-protocol, e.g:
    // https://sites.uci.edu/camp2014/2014/04/30/managing-midi-pitchbend-messages/
    slider_pitch->setRange(0, 16383);
    slider_pitch->setValue(8192);
    
    grid->addWidget(label_tether, 0, 0);
    grid->addWidget(label_pitch, 0, 1);
    grid->addWidget(slider_tether, 1, 0);
    grid->addWidget(slider_pitch, 1, 1);
    
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
    slider_tether->setStyleSheet(style);
    slider_pitch->setStyleSheet(style);
}
