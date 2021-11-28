#ifndef ORGELWERK_H
#define ORGELWERK_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QThread>
#include <QPixmap>
#include <QIcon>
#include <QDialog>
#include <QSizePolicy>

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
    void stopAllPressed();
    QPushButton *button_panic = new QPushButton;
    void movePitchWheel(int key);
    void keySustain(bool pressed);
    void keySostenuto(bool pressed);
    void keySoft(bool pressed);
    
    MIDIChannelSelector *channels;
    MIDIKeySelector *keys;
    MIDIPitchWheel *pitch;
    KeyboardPiano *piano;
    KeyboardPC *pc;
    
private:
    void drawGUI();
    void initInputThread();
    
    QGridLayout *grid;
    
    QSlider *slider_volume_master;
    QPushButton *button_channels;
    QScrollArea *scroll_channels;
    
    //InputKeyboardRaw *input_raw;
    QThread *thread_input;
    InputKeyboardRawThread *worker_input;
    
    InterfaceAudio *interface_audio;
    
    void keyMIDIHelper(int midicode, QString mode);
    
    // shows midi_channel_selector as a preview with a button to click to show the real widget
    // (saves screen space)
    void showChannelsImage(int grid_row);
    // shows directly the real widget (needs more screen space)
    void showChannelsReal(int grid_row);
    void updateChannelsSchreenshot();
    
private slots:
    void keyMIDIDown(int midicode);
    void keyMIDIUp(int midicode);
    void pitchWheelMoved(int pitch);
    void showChannelDetails(bool update_preview=false);
    void channelsDialogRejected();
    
protected:
    
    
signals:
    
};

#endif // ORGELWERK_H
