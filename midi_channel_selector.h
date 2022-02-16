#ifndef MIDICHANNELSELECTOR_H
#define MIDICHANNELSELECTOR_H

#include <QObject>
#include <QWidget>

#include <QGridLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QDial>
#include <QComboBox>
#include <QLabel>
#include <QEvent>
#include <QKeyEvent>
#include <QVariant>

#include "midi_sounds_list.h"
#include "interface_audio.h"
#include "midi_key_shift_widget.h"

class MIDIChannelSelector : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIChannelSelector(QList<InterfaceAudio*> list_of_audio_interfaces, QWidget *parent = nullptr);
    
    void setListOfAudioOutputs(QList<InterfaceAudio*> list_of_audio_interfaces);
    
    QList<QMap<QString, QVariant> > listOfChannels(bool only_activated=true);
    void setListOfChannels(QList<QMap<QString, QVariant> > data);
    void volumeDCAChanged(int value);
    void resendMIDIControls();
    
private:
    MIDISoundsList *midi_sounds_list = new MIDISoundsList;
    //InterfaceAudio *audio;
    QList<InterfaceAudio*> list_of_midi_outputs;
    
    int volume_dca = 100;
    
    void drawGUI();
    
    QString ADD_NEW_AUDIO_OUTPUT_LABEL = "[add new interface]";
    
    InterfaceAudio* selectedAudioInterface(int channel);
    
    QList<QCheckBox*> list_of_checkboxes;
    QList<QComboBox*> list_of_midi_output_combos;
    void populateAudioCombos();
    QList<MIDIKeyShiftWidget*> list_of_keyshifts;
    QList<QSpinBox*> list_of_key_mins;
    QList<QSpinBox*> list_of_key_maxs;
    QList<QSlider*> list_of_volume_sliders;
    QList<QSlider*> list_of_pan_sliders;
    QList<QComboBox*> list_of_instrument_groups;
    QList<QComboBox*> list_of_instrument_banks;
    QList<QSpinBox*> list_of_msb;
    QList<QSpinBox*> list_of_lsb;
    QList<QSlider*> list_of_portamentos;
    QList<QSlider*> list_of_attacks;
    QList<QSlider*> list_of_releases;
    
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    
private slots:
    void addNewAudioInterface(QString text);
    void volumeSliderMoved(int channel, int volume);
    void panSliderMoved(int channel, int value);
    void instrumentGroupChanged(int channel, QComboBox *combo_group, QComboBox *combo_instrument);
    void instrumentChanged(int channel, QString instrument);
    void instrumentChangedNumeric(int channel, int instrument_msb, int instrument_lsb);
    void portamentoChanged(int channel, int value);
    void attackChanged(int channel, int value);
    void releaseChanged(int channel, int value);
    void playTestNote();
    void stopTestNote();
    
signals:
    void newAudioInterfaceRequested(QString label);
    void eventFiltered(QObject *obj, QEvent *ev);
};

#endif // MIDICHANNELSELECTOR_H
