#include "interface_jack.h"

InterfaceJack::InterfaceJack(QString label, InterfaceAudio *parent) : InterfaceAudio(label, parent)
{
    this->label_string = "jack-midi-"+label;
    
    int err;
    
    this->jack_client = jack_client_open(this->NAME.toLatin1(), JackNoStartServer, NULL);
    
    if (this->jack_client == NULL) {
        qDebug() << "Could not connect to the JACK server; run jackd first?";
        //exit(EX_UNAVAILABLE);
    }
    
    //this->ringbuffer = jack_ringbuffer_create(RINGBUFFER_SIZE);
    /*
    if (ringbuffer == NULL) {
        qDebug() << "Cannot create JACK ringbuffer.";
        //exit(EX_SOFTWARE);
    }
    
    jack_ringbuffer_mlock(ringbuffer);
    
    err = jack_set_process_callback(jack_client, process_callback, 0);
    if (err) {
        qDebug() << "Could not register JACK process callback.";
        //exit(EX_UNAVAILABLE);
    }
    
    err = jack_set_graph_order_callback(jack_client, graph_order_callback, 0);
    if (err) {
        qDebug() << "Could not register JACK graph order callback.";
        //exit(EX_UNAVAILABLE);
    }
    
    output_port = jack_port_register(jack_client, OUTPUT_PORT_NAME, JACK_DEFAULT_MIDI_TYPE,
        JackPortIsOutput, 0);
    
    if (output_port == NULL) {
        qDebug() << "Could not register JACK output port.";
        //exit(EX_UNAVAILABLE);
    }
    
    input_port = jack_port_register(jack_client, INPUT_PORT_NAME, JACK_DEFAULT_MIDI_TYPE,
        JackPortIsInput, 0);
    
    if (input_port == NULL) {
        qDebug() << "Could not register JACK input port.";
        //exit(EX_UNAVAILABLE);
    }
    
    if (jack_activate(jack_client)) {
        qDebug() << "Cannot activate JACK client.";
        //exit(EX_UNAVAILABLE);
    }
    */
}

InterfaceJack::~InterfaceJack()
{
    
}

QString InterfaceJack::label()
{
    return "";
}

void InterfaceJack::keyPressEvent(int channel, int midicode)
{
    Q_UNUSED(channel);
    Q_UNUSED(midicode);
    
    qDebug() << "jack pressed: "+QString::number(midicode);
}

void InterfaceJack::keyReleaseEvent(int channel, int midicode)
{
    Q_UNUSED(channel);
    Q_UNUSED(midicode);
    
    qDebug() << "jack released: "+QString::number(midicode);
}

void InterfaceJack::keyPanicEvent(int channel)
{
    Q_UNUSED(channel);
}

void InterfaceJack::keyStopAllEvent(int channel)
{
    Q_UNUSED(channel);
}

void InterfaceJack::keyPitchbendEvent(int channel, int pitch)
{
    Q_UNUSED(channel);
    Q_UNUSED(pitch);
}

void InterfaceJack::keySustainEvent(int channel, bool pressed)
{
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceJack::keySostenutoEvent(int channel, bool pressed)
{
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceJack::keySoftEvent(int channel, bool pressed)
{
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceJack::setProgramChangeEvent(int channel, int program, int bank)
{
    Q_UNUSED(channel);
    Q_UNUSED(program);
    Q_UNUSED(bank);
}

void InterfaceJack::setVolumeChangeEvent(int channel, int volume)
{
    Q_UNUSED(channel);
    Q_UNUSED(volume);
}

void InterfaceJack::setPanChangeEvent(int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceJack::setPortamentoChanged(int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceJack::setAttackChanged(int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceJack::setReleaseChanged(int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceJack::setTremoloChanged(int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}
