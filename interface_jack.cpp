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
}

void InterfaceJack::createNewPort(QString label)
{
    if (jack_deactivate(this->jack_client)) {
        qDebug() << "Cannot deactivate JACK client.";
        //exit(EX_UNAVAILABLE);
    }
    
    this->output_port = jack_port_register(this->jack_client, label.toLocal8Bit(), JACK_DEFAULT_MIDI_TYPE,
        JackPortIsOutput, 0);
    
    if (this->output_port == NULL) {
        qDebug() << "Could not register JACK output port.";
        //exit(EX_UNAVAILABLE);
    }
    
    this->list_of_output_ports.append(this->output_port);
    
    this->input_port = jack_port_register(this->jack_client, label.toLocal8Bit(), JACK_DEFAULT_MIDI_TYPE,
        JackPortIsInput, 0);
    
    if (this->input_port == NULL) {
        qDebug() << "Could not register JACK input port.";
        //exit(EX_UNAVAILABLE);
    }
    
    this->list_of_input_ports.append(this->input_port);
    
    jack_set_process_callback(this->jack_client, jack_static_callback, (void *)this);
    
    this->map_of_ports[this->new_port_counter] = output_port;
    this->new_port_counter++;
    
    if (jack_activate(this->jack_client)) {
        qDebug() << "Cannot activate JACK client.";
        //exit(EX_UNAVAILABLE);
    }
}

InterfaceJack::~InterfaceJack()
{
    //jack_ringbuffer_free(this->ringbuffer);
    jack_client_close(this->jack_client);
}

int InterfaceJack::jack_static_callback(jack_nframes_t nframes, void *arg)
{
    ((InterfaceJack *) arg)->jack_process_midi_input(nframes);
    ((InterfaceJack *) arg)->jack_process_midi_output(nframes);
    
    return 0;
    //return ((InterfaceJack *) arg)->jack_process_midi_input(nframes);
}

int InterfaceJack::jack_process_midi_input(jack_nframes_t nframes)
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
int InterfaceJack::jack_process_midi_output(jack_nframes_t nframes)
{
    int read, t, bytes_remaining;
    unsigned char *buffer;
    void *port_buffer;
    jack_nframes_t last_frame_time;
    struct MidiMessage ev;
    
    last_frame_time = jack_last_frame_time(this->jack_client);
    
    port_buffer = jack_port_get_buffer(this->output_port, nframes);
    if (port_buffer == NULL)
    {
        qDebug() << "jack_port_get_buffer failed, cannot send anything";
        return 0;
    }
    
    jack_midi_clear_buffer(port_buffer);
    
    // we may push at most one byte per 0.32ms to stay below the 31.25 kbaud limit.
    bytes_remaining = jack_nframes_to_ms(nframes) * this->RATE_LIMIT;
    
    while (jack_ringbuffer_read_space(this->ringbuffer))
    {
        read = jack_ringbuffer_peek(ringbuffer, (char *)&ev, sizeof(ev));
        
        if (read != sizeof(ev))
        {
            qDebug() << "short read from ringbuffer, possible note loss";
            continue;
        }
        
        bytes_remaining -= ev.len;
        
        if (this->RATE_LIMIT > 0.0 && bytes_remaining <= 0)
        {
            qDebug() << "rate limit in effect";
            break;
        }
        
        t = ev.time + nframes - last_frame_time;
        
        // if computed time is too much into the future, we need to send it later
        if (t >= (int)nframes)
            break;
        
        // if computed time is < 0, we missed a cycle because of xrun
        if (t < 0)
            t = 0;
        
        if (this->TIME_OFFSETS_ARE_ZERO)
            t = 0;
        
        jack_ringbuffer_read_advance(this->ringbuffer, sizeof(ev));
        
        buffer = jack_midi_event_reserve(port_buffer, t, ev.len);
        
        if (buffer == NULL)
        {
            qDebug() << "jack_midi_event_reserve failed, NOTE LOST";
            break;
        }
        
        memcpy(buffer, ev.data, ev.len);
    }
}
double InterfaceJack::jack_nframes_to_ms(jack_nframes_t nframes)
{
    jack_nframes_t sr;
    
    sr = jack_get_sample_rate(this->jack_client);
    
    assert(sr > 0);
    
    return ((nframes * 1000.0) / (double)sr);
}

QString InterfaceJack::label()
{
    return "";
}

void InterfaceJack::keyPressEvent(int port, int channel, int midicode)
{
    qDebug() << "jack pressed: "+QString::number(midicode) << " channel: " << channel;
    
    sendEvent(port, "0x90", channel, midicode, 127);
}

void InterfaceJack::keyReleaseEvent(int port, int channel, int midicode)
{
    Q_UNUSED(channel);
    Q_UNUSED(midicode);
    
    qDebug() << "jack released: "+QString::number(midicode);
    
    sendEvent(port, "0x80", channel, midicode, 0);
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

void InterfaceJack::saveMIDISettings()
{
    qDebug() << "SAVING MIDI SETTINGS";
    for (int i=0; i < this->list_of_input_ports.length(); i++)
    {
        qDebug() << jack_port_type(this->list_of_output_ports[i]);
        
        const char **ports = jack_port_get_all_connections(this->jack_client, this->list_of_output_ports[i]);
        if (ports != NULL)
        {
            qDebug() << "Connections for " << this->list_of_input_ports[i] << ":";
            
        }
        //qDebug() << "Connections for " << this->list_of_input_ports[i] << ":";
    }
}
void InterfaceJack::loadMIDISettings()
{
    
}

void InterfaceJack::sendEvent(int port, QString opcode, int channel, int value, int velocity)
{
    // https://github.com/falkTX/jack-midi-timing/blob/master/sender.c
    
    unsigned char value_ = *(unsigned char*)(QString().number(value, 16).prepend("0x").toStdString().c_str());
    unsigned char velocity_ = *(unsigned char*)(QString().number(velocity, 16).prepend("0x").toStdString().c_str());
    //auto index_ = QByteArray::fromHex(QString::number(index));
    //QString value_ = QByteArray::fromHex(QString::number(value));
    
    unsigned int op = opcode.toUInt(nullptr, 16) + channel;
    unsigned char type = *(unsigned char*)(QString().number(op, 16).prepend("0x").toStdString().c_str());
    qDebug() << "type: " << type << " value: " << value_ << " velocity: " << velocity_;
    
    //const jack_midi_data_t data[3] = { type, value_, velocity_ };
    const jack_midi_data_t data[3] = { op, value, velocity };
    qDebug() << data;
    
    /*
    uint res = opcode.toUInt(nullptr, 16) + channel;
    QString res_ = QString().number(res, 16).prepend("0x").toStdString().c_str();
    QByteArray ba = QByteArray::fromHex(res_.toLatin1());
    //unsigned char type = reinterpret_cast<unsigned char>(ba.data());
    qDebug() << "res: " << res << " type: " << " ba: " << ba;
    */
    
    //jack_nframes_t event_index = 0;
    //jack_nframes_t event_index = jack_frames_since_cycle_start(this->jack_client);
    jack_nframes_t event_index = jack_midi_get_event_count(this->jack_client);
    
    void *output_port_buffer = jack_port_get_buffer(this->map_of_ports[port], event_index);
    jack_midi_clear_buffer(output_port_buffer);
    //jack_midi_event_write(output_port_buffer, event_index, data, sizeof(data));
    //const char* data[3] =  {op, value, velocity};
    
    struct MidiMessage ev;
    ev.data[0] = op;
    ev.data[1] = value;
    ev.data[2] = velocity;
    ev.time = jack_frame_time(this->jack_client);
    
    int written;
    
    if (jack_ringbuffer_write_space(this->ringbuffer) < sizeof(ev))
    {
        qDebug() << "not enough space in the ringbuffer, NOTE LOST";
    }
    
    written = jack_ringbuffer_write(this->ringbuffer, (char *)&ev, sizeof(ev));
    
    if (written != sizeof(ev))
    {
        qDebug() << "jack_ringbuffer_write failed, NOTE LOST";
    }
}
