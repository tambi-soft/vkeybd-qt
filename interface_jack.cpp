#include "interface_jack.h"

InterfaceJack::InterfaceJack(InterfaceAudio *parent) : InterfaceAudio(parent)
{
    //this->label_string = "jack-midi-"+label;
    
    int err;
    
    this->jack_client = jack_client_open(this->NAME.toLatin1(), JackNoStartServer, NULL);
    
    if (this->jack_client == NULL) {
        qDebug() << "Could not connect to the JACK server; run jackd first?";
        //exit(EX_UNAVAILABLE);
    }
    
    this->ringbuffer = jack_ringbuffer_create(RINGBUFFER_SIZE);
    if (this->ringbuffer == NULL) {
        qDebug() << "Cannot create JACK ringbuffer.";
        //exit(EX_SOFTWARE);
    }
    else
    {
        jack_ringbuffer_mlock(this->ringbuffer);
    }
    
    
    /*
    err = jack_set_process_callback(jack_client, process_callback, 0);
    if (err) {
        qDebug() << "Could not register JACK process callback.";
        //exit(EX_UNAVAILABLE);
    }
    */
    
    /*
    err = jack_set_graph_order_callback(jack_client, graph_order_callback, 0);
    if (err) {
        qDebug() << "Could not register JACK graph order callback.";
        //exit(EX_UNAVAILABLE);
    }
    */
    
    
    //this->createNewPort("bla");
    
    /*
    if (jack_activate(this->jack_client)) {
        qDebug() << "Cannot activate JACK client.";
        //exit(EX_UNAVAILABLE);
    }
    
    */
}

void InterfaceJack::createNewPort(QString label)
{
    qDebug() << "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR: " << label;
    this->output_port = jack_port_register(this->jack_client, label.toLocal8Bit(), JACK_DEFAULT_MIDI_TYPE,
        JackPortIsOutput, 0);
    
    if (this->output_port == NULL) {
        qDebug() << "Could not register JACK output port.";
        //exit(EX_UNAVAILABLE);
    }
    
    this->input_port = jack_port_register(this->jack_client, label.toLocal8Bit(), JACK_DEFAULT_MIDI_TYPE,
        JackPortIsInput, 0);
    
    if (this->input_port == NULL) {
        qDebug() << "Could not register JACK input port.";
        //exit(EX_UNAVAILABLE);
    }
    
    jack_set_process_callback(this->jack_client, jack_static_callback, (void *)this);
    
    Q_UNUSED(label);
}

QMap<int,QString> InterfaceJack::getPorts()
{
    
}

InterfaceJack::~InterfaceJack()
{
    //jack_ringbuffer_free(this->ringbuffer);
    jack_client_close(this->jack_client);
}

int InterfaceJack::jack_static_callback(jack_nframes_t nframes, void *arg)
{
  return ((InterfaceJack *) arg)->jack_callback(nframes);
}

int InterfaceJack::jack_callback(jack_nframes_t nframes)
{
    jack_midi_event_t ev;
    jack_nframes_t ev_count;
    int type, ch, index, val;
    
    void* midiBuf = jack_port_get_buffer(this->input_port, nframes);
    ev_count = jack_midi_get_event_count(midiBuf);
    while (ev_count) {
        ev_count--;
        jack_midi_event_get(&ev, midiBuf, ev_count);
        type = *ev.buffer & 0xf0;
        ch = *ev.buffer & 0x0f;
        index = *(ev.buffer + 1);
        val = *(ev.buffer + 2);
        
        qDebug() << "buffer: " << &ev.buffer;
        qDebug() << "type: " << type << " ch: " << ch << " index: " << index << " val: " << val;
        emit midiEvent(type, ch, index, val);
        
        //keyPressEvent(0, 0);
    }   
    return 0;
}

QString InterfaceJack::label()
{
    return "";
}

void InterfaceJack::keyPressEvent(int port, int channel, int midicode)
{
    qDebug() << "jack pressed: "+QString::number(midicode) << " channel: " << channel;
    
    sendEvent("0x90", channel, midicode, 127);
}

void InterfaceJack::keyReleaseEvent(int port, int channel, int midicode)
{
    Q_UNUSED(channel);
    Q_UNUSED(midicode);
    
    qDebug() << "jack released: "+QString::number(midicode);
    
    sendEvent("0x80", channel, midicode, 0);
}

void InterfaceJack::keyPanicEvent(int port, int channel)
{
    Q_UNUSED(channel);
}

void InterfaceJack::keyStopAllEvent(int port, int channel)
{
    Q_UNUSED(channel);
}

void InterfaceJack::keyPitchbendEvent(int port, int channel, int pitch)
{
    Q_UNUSED(channel);
    Q_UNUSED(pitch);
    
    
}

void InterfaceJack::keySustainEvent(int port, int channel, bool pressed)
{
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceJack::keySostenutoEvent(int port, int channel, bool pressed)
{
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceJack::keySoftEvent(int port, int channel, bool pressed)
{
    Q_UNUSED(channel);
    Q_UNUSED(pressed);
}

void InterfaceJack::setProgramChangeEvent(int port, int channel, int program, int bank)
{
    Q_UNUSED(channel);
    Q_UNUSED(program);
    Q_UNUSED(bank);
}

void InterfaceJack::setVolumeChangeEvent(int port, int channel, int volume)
{
    Q_UNUSED(channel);
    Q_UNUSED(volume);
}

void InterfaceJack::setPanChangeEvent(int port, int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceJack::setPortamentoChanged(int port, int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceJack::setAttackChanged(int port, int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceJack::setReleaseChanged(int port, int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceJack::setTremoloChanged(int port, int channel, int value)
{
    Q_UNUSED(channel);
    Q_UNUSED(value);
}

void InterfaceJack::sendEvent(QString opcode, int channel, int value, int velocity)
{
    // https://github.com/falkTX/jack-midi-timing/blob/master/sender.c
    
    //jack_nframes_t event_index = 0;
    jack_nframes_t event_index = jack_frames_since_cycle_start(this->jack_client);
    
    jack_nframes_t nframes = 0;
    void *output_port_buffer = jack_port_get_buffer(this->output_port, nframes);
    jack_midi_clear_buffer(output_port_buffer);
    
    unsigned char value_ = *(unsigned char*)(QString().number(value, 16).prepend("0x").toStdString().c_str());
    unsigned char velocity_ = *(unsigned char*)(QString().number(velocity, 16).prepend("0x").toStdString().c_str());
    //auto index_ = QByteArray::fromHex(QString::number(index));
    //QString value_ = QByteArray::fromHex(QString::number(value));
    
    /*
    uint res = opcode.toUInt(nullptr, 16) + channel;
    QString res_ = QString().number(res, 16).prepend("0x").toStdString().c_str();
    QByteArray ba = QByteArray::fromHex(res_.toLatin1());
    //unsigned char type = reinterpret_cast<unsigned char>(ba.data());
    qDebug() << "res: " << res << " type: " << " ba: " << ba;
    */
    
    unsigned int op = opcode.toUInt(nullptr, 16) + channel;
    unsigned char type = *(unsigned char*)(QString().number(op, 16).prepend("0x").toStdString().c_str());
    qDebug() << "type: " << type << " value: " << value_ << " velocity: " << velocity_;
    
    //const jack_midi_data_t data[3] = { type, value_, velocity_ };
    const jack_midi_data_t data[3] = { op, value, velocity };
    //qDebug() << data;
    jack_midi_event_write(output_port_buffer, event_index, data, sizeof(data));
    
    //jack_midi_clear_buffer(output_port_buffer);
}
