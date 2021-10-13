#ifndef ORGELWERK_H
#define ORGELWERK_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>
#include <QGroupBox>

#include "midi_channel_selector.h"
#include "midi_key_selector.h"
#include "keyboard_piano.h"
#include "keyboard_pc.h"

class Orgelwerk : public QWidget
{
    Q_OBJECT
public:
    explicit Orgelwerk(QWidget *parent = nullptr);
    
private:
    MIDIChannelSelector *channels;
    MIDIKeySelector *keys;
    KeyboardPiano *piano;
    KeyboardPC *pc;
    
signals:
    
};

#endif // ORGELWERK_H
