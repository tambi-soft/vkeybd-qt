#include "midi_sounds_list.h"

MIDISoundsList::MIDISoundsList()
{
    
}

QStringList MIDISoundsList::getCategories()
{
    QStringList list;
    list << "Piano"
         << "Chromatic Percussion"
         << "Organ"
         << "Guitar"
         << "Bass"
         << "Strings"
         << "Orchestral Ensemble"
         << "Brass"
         << "Reed"
         << "Wind"
         << "Synth Lead"
         << "Synth Pad"
         << "Synth Effects"
         << "Ethnic"
         << "Percussive"
         << "Sound Effects";
    return list;
}

void MIDISoundsList::getInstruments()
{
    
}
