#ifndef ORGELWERK_H
#define ORGELWERK_H

#include <QObject>

#include <QThread>
#include <QTimer>
#include <QRandomGenerator>
class TremoloWorker : public QObject
{
    Q_OBJECT
public:
    explicit TremoloWorker(int interface_index, int delay, int channel, int note, QObject *parent = 0);
    
private:
    int interface_index;
    QTimer *timer;
    int delay;
    int channel;
    int note;
    
    QRandomGenerator *random = new QRandomGenerator;
    
public slots:
    void tick();
    
signals:
    void notePlay(int interface_index, int channel, int note);
    void noteStop(int interface_index, int channel, int note);
    
};





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
#include <QSpinBox>

#include "midi_channel_selector.h"
#include "midi_key_selector.h"
#include "midi_pitchwheel.h"
#include "midi_channels_summary.h"
#include "midi_key_shift_widget.h"
#include "midi_mastervolume.h"
#include "keyboard_piano.h"
#include "keyboard_pc.h"
#include "keyboard_notes.h"

#include "interface_audio.h"
#include "interface_alsa.h"
#include "interface_jack.h"

#include "enums_structs.h"

class Orgelwerk : public QWidget
{
    Q_OBJECT
public:
    explicit Orgelwerk(int keyboard_id, int tab_id, InterfaceAudio *interface_audio, QString label, QWidget *parent = nullptr);
    ~Orgelwerk();
    
    void keyDownRaw(int keycode);
    void keyUpRaw(int keycode);
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
    MIDIMasterVolume *volume;
    KeyboardNotes *notes = nullptr;
    KeyboardPiano *piano = nullptr;
    KeyboardPC *pc = nullptr;
    
    QSpinBox *spin_udp_port;
    QPushButton *button_channels_dialog = new QPushButton;
    QPushButton *button_resend_midi = new QPushButton;
    QCheckBox *check_resend_midi_auto = new QCheckBox("auto");
    
    QList<QMap<QString,QVariant>> listOfChannels(bool only_activated);
    QMap<QString, QVariant> getParams();
    void restoreParams(QMap<QString,QVariant> data);
    
    void showHideGUIElements(GUIElements elements, bool show);
    
    void octaveDown();
    void octaveUp();
    void globalPitchShiftChanged(int value, bool is_relative=false);
    
    void activated();
    
private:
    int keyboard_id;
    int tab_id;
    QString label;
    
    void drawGUI();
    void drawNotesKeyboard(int grid_row);
    void drawPianoKeyboard(int grid_row);
    void drawPCKeyboard(int grid_row);
    
    QGroupBox *group_keys;
    QGroupBox *group_pitch;
    
    void notePlay(int interface_index, int channel, int note);
    void noteStop(int interface_index, int channel, int note);
    
    QGridLayout *grid;
    
    QPushButton *button_channels;
    QScrollArea *scroll_channels;
    MIDIChannelsSummary *midi_channels_summary;
    MIDIKeyShiftWidget *key_shift_master;
    
    //QThread *thread_input;
    InterfaceAudio *interface_audio;
    QMap<QString, TremoloWorker*> map_of_tremolo_workers;
    QMap<QString, QThread*> map_of_tremolo_threads;
    
    // We want to be able to switch between tabs and keys smoothly during play
    // The desired behaviour is:
    // If a key is pressed and after than the tab or the key is switched,
    // than the key-up signal has to be sent to the old tab/key, too.
    // With map_of_keys_down we keep track of witch key is currently pressed and for which key.
    QMap<int, QList<int>> map_of_keys_down;
    void keyShiftMapAdd(int keycode);
    void keyShiftMapRemove(int key, int keycode);
    
    void keyMIDIHelper(int midicode, MIDIMode mode);
    void tremoloThreadStart(int interface_index, int channel, int m_code_shifted, int tremolo);
    void tremoloThreadStop(int channel, int m_code_shifted);
    
    void showChannelsSummary(int grid_row);
    void channelsSummaryUpdate();
    
private slots:
    void addNewAudioInterface(QString label);
    
    void keyMIDIDown(int midicode);
    void keyMIDIUp(int midicode);
    void pitchWheelMoved(int pitch);
    void showChannelDetails();
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
