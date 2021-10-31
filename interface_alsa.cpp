#include "interface_alsa.h"

/*
 * http://jack-keyboard.sourceforge.net/
 * https://stackoverflow.com/questions/21161118/send-midi-messages-from-c
 * https://tldp.org/HOWTO/MIDI-HOWTO-9.html
 * https://www.alsa-project.org/alsa-doc/alsa-lib/group___seq_middle.html
 * https://www.alsa-project.org/alsa-doc/alsa-lib/seq.html
 */

InterfaceAlsa::InterfaceAlsa(QString label, InterfaceAudio *parent) : InterfaceAudio(parent)
{
    this->label = label;
    
    qDebug() << "init interface alsa: "+this->label;
    
    snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0);
    snd_seq_set_client_name(seq, "vkeybd-qt");
    
    int port;
    port = snd_seq_create_simple_port(seq, this->label.toLatin1(),
        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ | SND_SEQ_PORT_CAP_WRITE,
        SND_SEQ_PORT_TYPE_APPLICATION);
    
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_direct(&ev);
}

void InterfaceAlsa::keyPressEvent(int midicode)
{
    qDebug() << "alsa pressed: "+QString::number(midicode);
    
    /* either */
    //snd_seq_ev_set_dest(&ev, 64, 0); /* send to 64:0 */
    /* or */
    snd_seq_ev_set_subs(&ev);        /* send to subscribers of source port */

    snd_seq_ev_set_noteon(&ev, 0, midicode, 127);
    snd_seq_event_output(seq, &ev);
    
    snd_seq_drain_output(seq);
}

void InterfaceAlsa::keyReleaseEvent(int midicode)
{
    snd_seq_ev_set_noteoff(&ev, 0, midicode, 127);
    
    snd_seq_event_output(seq, &ev);
    snd_seq_drain_output(seq);
}

void InterfaceAlsa::keyPanicEvent()
{
    snd_seq_ev_set_controller(&ev, 0, MIDI_CTL_ALL_NOTES_OFF, 127);
    
    snd_seq_event_output(seq, &ev);
    snd_seq_drain_output(seq);
}

void InterfaceAlsa::keyPitchbendEvent(int pitch)
{
    snd_seq_ev_set_pitchbend(&ev, 0, pitch);
    
    snd_seq_event_output(seq, &ev);
    snd_seq_drain_output(seq);
}
