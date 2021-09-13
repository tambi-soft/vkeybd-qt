#include "alsa_adapter.h"

/*
 * http://jack-keyboard.sourceforge.net/
 * https://stackoverflow.com/questions/21161118/send-midi-messages-from-c
 * https://tldp.org/HOWTO/MIDI-HOWTO-9.html
 * https://www.alsa-project.org/alsa-doc/alsa-lib/group___seq_middle.html
 * https://www.alsa-project.org/alsa-doc/alsa-lib/seq.html
 */

AlsaAdapter::AlsaAdapter(QObject *parent) : QObject(parent)
{
    snd_seq_t *seq;
    /*
    snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0);
    
    int port;
    port = snd_seq_create_simple_port(seq, "my port",
        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_WRITE,
        SND_SEQ_PORT_TYPE_APPLICATION);
    */
}
