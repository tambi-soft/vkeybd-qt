#ifndef ORGELWERK_H
#define ORGELWERK_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QThread>

#include "midi_channel_selector.h"
#include "midi_key_selector.h"
#include "midi_pitchwheel.h"
#include "keyboard_piano.h"
#include "keyboard_pc.h"
#include "input_keyboard_raw_thread.h"

class Orgelwerk : public QWidget
{
    Q_OBJECT
public:
    explicit Orgelwerk(QWidget *parent = nullptr);
    ~Orgelwerk();
    
private:
    MIDIChannelSelector *channels;
    MIDIKeySelector *keys;
    MIDIPitchWheel *pitch;
    KeyboardPiano *piano;
    KeyboardPC *pc;
    
    void drawGUI();
    void initInputThread();
    
    //InputKeyboardRaw *input_raw;
    QThread *thread_input;
    InputKeyboardRawThread *worker_input;
    
signals:
    
};

#endif // ORGELWERK_H
