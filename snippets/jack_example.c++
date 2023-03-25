#include
#include

#include

// JACK MIDI output port name
#define MIDI_OUT_PORT_NAME "my_midi_out"

int main(int argc, char **argv)
{
    // Initialize JACK client
    jack_client_t *client = jack_client_open("MyMIDIApp", JackNoStartServer, NULL);
    if (!client) {
        std::cerr << "Failed to open JACK client" << std::endl;
        return -1;
    }

    // Register JACK MIDI output port
    jack_port_t *midi_out_port = jack_port_register(client, MIDI_OUT_PORT_NAME, JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
    if (!midi_out_port) {
        std::cerr << "Failed to register JACK MIDI output port" << std::endl;
        jack_client_close(client);
        return -1;
    }

    // Activate the client
    if (jack_activate(client)) {
        std::cerr << "Failed to activate JACK client" << std::endl;
        jack_client_close(client);
        return -1;
    }

    // Send some MIDI notes
    jack_midi_event_t midi_event;
    jack_midi_data_t midi_data[3];

    // Note on event
    midi_data[0] = 0x90; // Status byte
    midi_data[1] = 60; // Note number
    midi_data[2] = 100; // Velocity
    
    midi_event.time = 0; // Timestamp in frames
    midi_event.size = sizeof(midi_data); // Size of MIDI data
    midi_event.buffer = jack_midi_event_reserve(jack_port_midi_buffer(midi_out_port), 0, midi_event.size); // Reserve space in the buffer
    memcpy(midi_event.buffer, midi_data, sizeof(midi_data)); // Copy the MIDI data to the buffer
    jack_midi_event_write(jack_port_midi_buffer(midi_out_port), &midi_event); // Write the MIDI event to the buffer
    
    
    
    
    
    // Note off event
    midi_data[0] = 0x80; // Status byte
    midi_data[1] = 60; // Note number
    midi_data[2] = 0; // Velocity
    
    midi_event.time = 44100; // Timestamp in frames
    midi_event.size = sizeof(midi_data); // Size of MIDI data
    midi_event.buffer = jack_midi_event_reserve(jack_port_midi_buffer(midi_out_port), 0, midi_event.size); // Reserve space in the buffer
    memcpy(midi_event.buffer, midi_data, sizeof(midi_data)); // Copy the MIDI data to the buffer
    jack_midi_event_write(jack_port_midi_buffer(midi_out_port), &midi_event); // Write the MIDI event to the buffer

    // Disconnect the client and close JACK
    jack_deactivate(client);
    jack_client_close(client);

    return 0;
}
