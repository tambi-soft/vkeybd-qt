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

#include "interface_audio.h"
#include "interface_alsa.h"
#include "interface_jack.h"

class Orgelwerk : public QWidget
{
    Q_OBJECT
public:
    explicit Orgelwerk(QString label, QWidget *parent = nullptr);
    ~Orgelwerk();
    
    void keyDown(int keycode);
    void keyUp(int keycode);
    void panicKeyPressed();
    QPushButton *button_grab = new QPushButton;
    QPushButton *button_panic = new QPushButton;
    void movePitchWheel(int key);
    
    MIDIChannelSelector *channels;
    MIDIKeySelector *keys;
    MIDIPitchWheel *pitch;
    KeyboardPiano *piano;
    KeyboardPC *pc;
    
private:
    void drawGUI();
    void initInputThread();
    
    //InputKeyboardRaw *input_raw;
    QThread *thread_input;
    InputKeyboardRawThread *worker_input;
    
    InterfaceAudio *interface_audio;
    
    void keyMIDIHelper(int midicode, QString mode);
    
private slots:
    void keyMIDIDown(int midicode);
    void keyMIDIUp(int midicode);
    void pitchWheelMoved(int pitch);
    void volumeChanged(int channel, int volume);
    void instrumentChanged(int channel, int program, int bank);
    void grabKeyboardPressed();
    
protected:
    
    
signals:
    void grabKeyboardPressedSignal();
};

#endif // ORGELWERK_H
