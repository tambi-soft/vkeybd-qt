#include "interface_alsa.h"

/*
 * http://jack-keyboard.sourceforge.net/
 * https://stackoverflow.com/questions/21161118/send-midi-messages-from-c
 * https://tldp.org/HOWTO/MIDI-HOWTO-9.html
 * https://www.alsa-project.org/alsa-doc/alsa-lib/group___seq_middle.html
 * https://www.alsa-project.org/alsa-doc/alsa-lib/seq.html
 */

InterfaceAlsa::InterfaceAlsa(QString label, InterfaceAudio *parent) : InterfaceAudio(label, parent)
{
    this->label_string = label;
    
    snd_seq_open(&this->seq, "default", SND_SEQ_OPEN_DUPLEX, 0);
    snd_seq_set_client_name(this->seq, this->NAME.toLatin1());
    
    int port;
    port = snd_seq_create_simple_port(this->seq, this->label_string.toLatin1(),
        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ | SND_SEQ_PORT_CAP_WRITE,
        SND_SEQ_PORT_TYPE_APPLICATION);
    Q_UNUSED(port);
    
    snd_seq_ev_clear(&this->ev);
    snd_seq_ev_set_direct(&this->ev);
    
    /* either */
    //snd_seq_ev_set_dest(&this->ev, 64, 0); /* send to 64:0 */
    /* or */
    snd_seq_ev_set_subs(&this->ev);        /* send to subscribers of source port */
}

void InterfaceAlsa::createNewPort(QString label)
{
    Q_UNUSED(label);
}

InterfaceAlsa::~InterfaceAlsa()
{
    for (int i=0; i < 16; i++)
    {
        snd_seq_ev_set_controller(&this->ev, i, MIDI_CTL_ALL_NOTES_OFF, 127);
        sendEvent(true);
    }
    snd_seq_close(this->seq);
}

QString InterfaceAlsa::label()
{
    return this->label_string;
}

void InterfaceAlsa::keyPressEvent(int channel, int midicode)
{
    snd_seq_ev_set_noteon(&this->ev, channel, midicode, 127);
    
    sendEvent(true);
}

void InterfaceAlsa::keyReleaseEvent(int channel, int midicode)
{
    snd_seq_ev_set_noteoff(&this->ev, channel, midicode, 127);
    
    sendEvent(true);
}

void InterfaceAlsa::keyPanicEvent(int channel)
{
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_ALL_NOTES_OFF, 127);
    sendEvent(true);
}

void InterfaceAlsa::keyStopAllEvent(int channel)
{
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_ALL_SOUNDS_OFF, 127);
    sendEvent(true);
}

void InterfaceAlsa::keyPitchbendEvent(int channel, int pitch)
{
    // In the midi-specs 8192 is defined as the neutral center.
    // However in the ALSA docs it says:
    // val: pitch bend; zero centered from -8192 to 8191 
    pitch = pitch - 8192;
    
    snd_seq_ev_set_pitchbend(&this->ev, channel, pitch);
    
    sendEvent(true);
}

void InterfaceAlsa::keySustainEvent(int channel, bool pressed)
{
    int systain_value = 0;
    if (pressed)
        systain_value = 127;
    
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_SUSTAIN, systain_value);
    sendEvent(true);
}

void InterfaceAlsa::keySostenutoEvent(int channel, bool pressed)
{
    int systain_value = 0;
    if (pressed)
        systain_value = 127;
    
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_SOSTENUTO, systain_value);
    sendEvent(true);
}

void InterfaceAlsa::keySoftEvent(int channel, bool pressed)
{
    int value = 0;
    if (pressed)
        value = 127;
    
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_SOFT_PEDAL, value); // CC 67
    sendEvent(true);
}

void InterfaceAlsa::setProgramChangeEvent(int channel, int program, int bank)
{
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_MSB_BANK, 121);
    sendEvent(false);
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_LSB_BANK, bank);
    sendEvent(false);
    snd_seq_ev_set_pgmchange(&this->ev, channel, program);
    sendEvent(true);
}

void InterfaceAlsa::setVolumeChangeEvent(int channel, int volume)
{
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_MSB_MAIN_VOLUME, volume); // CC 7
    sendEvent(true);
}

void InterfaceAlsa::setPanChangeEvent(int channel, int value)
{
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_MSB_PAN, value);
    sendEvent(true);
}

void InterfaceAlsa::setPortamentoChanged(int channel, int value)
{
    if (value > 0)
    {
        snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_PORTAMENTO, 127); // CC 5
        sendEvent(true);
    }
    else
    {
        snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_PORTAMENTO, 0); // CC 5
        sendEvent(true);
    }
    
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_MSB_PORTAMENTO_TIME, value); // CC 65
    sendEvent(true);
}

void InterfaceAlsa::setAttackChanged(int channel, int value)
{
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_SC4_ATTACK_TIME, value);
    
    sendEvent(true);
}

void InterfaceAlsa::setReleaseChanged(int channel, int value)
{
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_SC3_RELEASE_TIME, value);
    
    sendEvent(true);
}

void InterfaceAlsa::setTremoloChanged(int channel, int value)
{
    snd_seq_ev_set_controller(&this->ev, channel, MIDI_CTL_E2_TREMOLO_DEPTH, value);
    
    sendEvent(true);
}

void InterfaceAlsa::sendEvent(bool drain)
{
    snd_seq_ev_set_direct(&this->ev);
    snd_seq_event_output(this->seq, &this->ev);
    
    if (drain)
    {
        snd_seq_drain_output(this->seq);
    }
}
