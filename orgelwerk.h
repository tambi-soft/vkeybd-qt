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
#include "keyboard_notes.h"

#include "input_keyboard_raw_thread.h"

#include "interface_audio.h"
#include "interface_alsa.h"
#include "interface_jack.h"
#include "midi_channels_summary.h"
#include "midi_key_shift_widget.h"

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
    QPushButton *button_stop_all = new QPushButton;
    void movePitchWheel(int key);
    void keySustain(bool pressed);
    void keySostenuto(bool pressed);
    void keySoft(bool pressed);
    
    MIDIChannelSelector *channels;
    MIDIKeySelector *keys;
    MIDIPitchWheel *pitch;
    KeyboardNotes *notes = nullptr;
    KeyboardPiano *piano = nullptr;
    KeyboardPC *pc = nullptr;
    
    QPushButton *button_channels_dialog = new QPushButton;
    QPushButton *button_resend_midi = new QPushButton;
    
    QList<QMap<QString,QVariant>> listOfChannels(bool only_activated);
    
private:
    void drawGUI();
    void drawNotesKeyboard(int grid_row);
    void drawPianoKeyboard(int grid_row);
    void drawPCKeyboard(int grid_row);
    void initInputThread();
    
    QGridLayout *grid;
    
    QLabel *label_volume_master;
    QSlider *slider_volume_master;
    QPushButton *button_channels;
    QScrollArea *scroll_channels;
    MIDIChannelsSummary *midi_channels_summary;
    MIDIKeyShiftWidget *key_shift_master;
    
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
    void showChannelsSummary(int grid_row);
    void channelsSummaryUpdate();
    void updateChannelsSchreenshot();
    
private slots:
    void keyMIDIDown(int midicode);
    void keyMIDIUp(int midicode);
    void pitchWheelMoved(int pitch);
    void showChannelDetails(bool update_preview=false);
    void channelsDialogRejected();
    void volumeSliderMoved(int value);
    
public slots:
    void resendMIDIControls();
    
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    
signals:
    void eventFiltered(QObject *obj, QEvent *ev);
    
};

#endif // ORGELWERK_H
