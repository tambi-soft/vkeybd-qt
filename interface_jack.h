#ifndef INTERFACEJACK_H
#define INTERFACEJACK_H

#include <QObject>
#include <QDebug>

#include "interface_audio.h"

#include <jack/jack.h>
#include <jack/midiport.h>
#include <jack/ringbuffer.h>

class InterfaceJack : public InterfaceAudio
{
    Q_OBJECT
public:
    explicit InterfaceJack(InterfaceAudio *parent = nullptr);
    ~InterfaceJack();
    
    QString label();
    
    void createNewPort(QString label);
    
    void keyPressEvent(int port, int channel, int midicode);
    void keyReleaseEvent(int port, int channel, int midicode);
    void keyPanicEvent(int port, int channel);
    void keyStopAllEvent(int port, int channel);
    void keyPitchbendEvent(int port, int channel, int pitch);
    void keySustainEvent(int port, int channel, bool pressed);
    void keySostenutoEvent(int port, int channel, bool pressed);
    void keySoftEvent(int port, int channel, bool pressed);
    void setProgramChangeEvent(int port, int channel, int program, int bank);
    void setVolumeChangeEvent(int port, int channel, int volume);
    void setPanChangeEvent(int port, int channel, int value);
    void setPortamentoChanged(int port, int channel, int value);
    void setAttackChanged(int port, int channel, int value);
    void setReleaseChanged(int port, int channel, int value);
    void setTremoloChanged(int port, int channel, int value);
    
    void saveMIDISettings();
    void loadMIDISettings();
    
    QString NAME = "vkeybd-qt";
    
private:
    struct MidiMessage
    {
        jack_nframes_t time;
        int len; // length of MIDI message in bytes
        unsigned char data[3];
    };
    //size_t RINGBUFFER_SIZE = 1024;
    size_t RINGBUFFER_SIZE = 1024 * sizeof(struct MidiMessage);
    double RATE_LIMIT = 0.0;
    int TIME_OFFSETS_ARE_ZERO = 0;
    
    jack_client_t *jack_client = NULL;
    jack_ringbuffer_t *ringbuffer;
    
    jack_port_t *output_port;
    jack_port_t *input_port;
    QList<jack_port_t*> list_of_output_ports;
    QList<jack_port_t*> list_of_input_ports;
    
    int new_port_counter = 0;
    QMap<int, jack_port_t*> map_of_ports;
    
    void sendEvent(int port, QString opcode, int channel, int value, int velocity);
    
    static int jack_static_callback(jack_nframes_t nframes, void *arg);
    int jack_process_midi_input(jack_nframes_t nframes);
    int jack_process_midi_output(jack_nframes_t nframes);
    double jack_nframes_to_ms(jack_nframes_t nframes);
    
signals:
    void midiEvent(int type, int ch, int index, int val);
};

#endif // INTERFACEJACK_H
