#include "midi_sounds_list.h"

MIDISoundsList::MIDISoundsList()
{
    
}

QList<QString> MIDISoundsList::getInstrumentsForGroupMIDIv1(QString group)
{
    return getInstrumentsForGroupHelper(group, false);
}
QList<QString> MIDISoundsList::getInstrumentsForGroupMIDIv2(QString group)
{
    return getInstrumentsForGroupHelper(group, true);
}
QList<QString> MIDISoundsList::getInstrumentsForGroupHelper(QString group, bool midi_v2)
{
    QList<QString> result;
    
    QList<QString> list_raw = getInstrumentsRAWData();
    
    bool in_group = false;
    for (int i=0; i < list_raw.length(); i++)
    {
        QString line = list_raw.at(i);
        
        if (line == group)
        {
            in_group = true;
        }
        else if (in_group)
        {
            if (!line.at(0).isDigit())
            {
                //in_group = false;
                break;
            }
            else
            {
                QList<QVariant> splitted = splitLine(line);
                
                if (splitted.at(1).toInt() == 0 || midi_v2)
                {
                    result.append(splitted.at(2).toString());
                }
            }
        }
    }
    
    //result.append(this->BANK_NO_NAME);
    
    return result;
}
QList<QVariant> MIDISoundsList::splitLine(QString line)
{
    QList<QVariant> result;
    
    QList<QString> splitted = line.split(" ", Qt::SkipEmptyParts);
    result.append(splitted.at(0));
    result.append(splitted.at(1));
    
    if (splitted.length() == 3)
    {
        result.append(splitted.at(2));
    }
    else
    {
        QString name;
        for (int i=2; i < splitted.length(); i++)
        {
            name.append(splitted.at(i));
            name.append(" ");
        }
        result.append(name.trimmed());
    }
    
    return result;
}

QList<QString> MIDISoundsList::getInstrumentGroups()
{
    QList<QString> result;
    
    QList<QString> list_raw = getInstrumentsRAWData();
    
    for (int i=0; i < list_raw.length(); i++)
    {
        QString line = list_raw.at(i);
        
        if (!line.at(0).isDigit())
        {
            result.append(line);
        }
    }
    
    return result;
}

QMap<QString,int> MIDISoundsList::getMIDICodesForInstrument(QString instrument)
{
    QMap<QString,int> result;
    result["msb"] = 0;
    result["lsb"] = 0;
    
    QList<QString> list_raw = getInstrumentsRAWData();
    for (int i=0; i < list_raw.length(); i++)
    {
        if (list_raw.at(i).contains(instrument))
        {
            QList<QString> splitted = list_raw.at(i).split(" ", Qt::SkipEmptyParts);
            
            //result << splitted.at(0).toInt() << splitted.at(1).toInt();
            result["msb"] = splitted.at(0).toInt();
            result["lsb"] = splitted.at(1).toInt();
            
            break;
        }
    }
    
    return result;
}

QMap<QString,QString> MIDISoundsList::getInstrumentForMIDICodes(int msb, int lsb)
{
    // prepare default result structure and values
    QMap<QString,QString> result;
    result["group"] = "";
    result["instrument"] = "";
    
    // compensate list starting with 1 and not 0
    ++msb;
    
    // determine the actual result values
    QList<QString> list_raw = getInstrumentsRAWData();
    bool breaked = false;
    for (int i=0; i < list_raw.length(); i++)
    {
        QString line = list_raw.at(i).trimmed();
        if (line.at(0).isDigit())
        {
            QList<QString> splitted = line.split(" ", Qt::SkipEmptyParts);
            if (splitted.at(0).toInt() == msb && splitted.at(1).toInt() == lsb)
            {
                for (int c=2; c < splitted.length(); c++)
                {
                    result["instrument"].append(splitted.at(c)+" ");
                }
                result["instrument"] = result["instrument"].trimmed();
                
                breaked = true;
                break;
            }
        }
        else
        {
            result["group"] = line;
        }
    }
    if (! breaked)
    {
        // if no match, we just set to empty
        result["group"] = "";
        result["instrument"] = "";
    }
    
    return result;
}

QList<QString> MIDISoundsList::getInstrumentsRAWData()
{
    // [msb, lsb, name]
    QList<QString> data = {
        "Piano",
        "1  0 Acoustic Grand Piano",
        "1  1 Wide Acoustic Grand",
        "1  2 Dark Acoustic Grand",
        "2  0 Bright Acoustic Piano",
        "2  1 Wide Bright Acoustic",
        "3  0 Electric Grand Piano",
        "3  1 Wide Electric Grand",
        "4  0 Honky-tonk Piano",
        "4  1 Wide Honky-tonk",
        "5  0 Rhodes Piano",
        "5  1 Detuned Electric Piano 1",
        "5  2 Electric Piano 1 Variation",
        "5  3 60's Electric Piano",
        "6  0 Chorused Electric Piano",
        "6  1 Detuned Electric Piano 2",
        "6  2 Electric Piano 2 Variation",
        "6  3 Electric Piano Legend",
        "6  4 Electric Piano Phase",
        "7  0 Harpsichord",
        "7  1 Coupled Harpsichord",
        "7  2 Wide Harpsichord",
        "7  3 Open Harpsichord",
        "8  0 Clavinet",
        "8  1 Pulse Clavinet",
        "Chromatic Percussion",
        "9  0 Celesta",
        "10 0 Glockenspiel",
        "11 0 Music Box",
        "12 0 Vibraphone",
        "12 1 Wet Vibraphone",
        "13 0 Marimba",
        "13 1 Wite Marimba",
        "14 0 Xylophone",
        "15 0 Tubular Bell",
        "15 1 Church Bell",
        "15 2 Carillon",       
        "16 0 Dulcimer/Santur",
        "Organ",
        "17 0 Drawbar Organ",
        "17 1 Detuned Organ 1",
        "17 2 60's Organ 1",
        "17 3 Organ 4",
        "18 0 Percussive B3 Organ",
        "18 1 Detuned Organ 2",
        "18 2 Organ 5",
        "19 0 Rock Organ",
        "20 0 Church Organ 1",
        "20 1 Church Organ 2",
        "20 2 Church Organ 3",
        "21 0 Reed Organ",
        "21 1 Puff Organ",
        "22 0 French Accordion",
        "22 1 Italian Accordion",
        "23 0 Harmonica",
        "24 0 Bandoneon",
        "Guitar",
        "25 0 Nylon-String Guitar",
        "25 1 Ukulele",
        "25 2 Open Nylon Guitar",
        "25 3 Nylon Guitar 2",
        "26 0 Steel-String Guitar",
        "26 1 12-String Guitar",
        "26 2 Mandolin",
        "26 3 Steel + Body",
        "27 0 Jazz Guitar",
        "27 1 Hawaiian Guitar",
        "28 0 Clean Electric Guitar",
        "28 1 Chorus Guitar",
        "28 2 Mid Tone Guitar",
        "29 0 Muted Electric Guitar",
        "29 1 Funk Guitar",
        "29 2 Funk Guitar 2",
        "29 3 Jazz Mandolin",
        "30 0 Overdriven Guitar",
        "30 1 Guitar Pinch",
        "31 0 Distortion Guitar",
        "31 1 Feedback Guitar",
        "31 2 Distortion Rtm Guitar",
        "32 0 Guitar Harmonics",
        "32 1 Guitar Feedback",
        "Bass",
        "33 0 Acoustic Bass",
        "34 0 Fingered Bass",
        "34 1 Finger Slap",
        "35 0 Picked Bass",
        "36 0 Fretless Bass",
        "37 0 Slap Bass 1",
        "38 0 Slap Bass 2",
        "39 0 Synth Bass 1",
        "39 1 Synth Bass 101",
        "39 2 Synth Bass 3",
        "39 3 Clavi Bass",
        "39 4 Hammer",
        "40 0 Synth Bass 2",
        "40 1 Synth Bass 4",
        "40 2 Rubber Bass",
        "40 3 Attack Pulse",
        "Orchestra Solo",
        "41 0 Violin",
        "41 1 Slow Violin",
        "42 0 Viola",
        "43 0 Cello",
        "44 0 Contrabass",
        "45 0 Tremolo Strings",
        "46 0 Pizzicato Strings",
        "47 0 Harp",
        "47 1 Yang Qin",
        "48 0 Timpani",
        "Orchestra Ensemble",
        "49 0 String Ensemble",
        "49 1 Orchestra Strings",
        "49 2 60's Strings",
        "50 0 Slow String Ensemble",
        "51 0 Synth Strings 1",
        "51 1 Synth Strings 3",
        "52 0 Synth Strings 2",
        "53 0 Choir Aahs",
        "53 1 Choir Aahs 2",
        "54 0 Voice Oohs",
        "54 1 Humming",
        "55 0 Synth Voice",
        "55 1 Analog Voice",
        "56 0 Orchestra Hit",
        "56 1 Bass Hit",
        "56 2 6th Hit",
        "56 3 Euro Hit",
        "Brass",
        "57 0 Trumpet",
        "57 1 Dark Trumpet",
        "58 0 Trombone",
        "58 1 Trombone 2",
        "58 2 Bright Trombone",
        "59 0 Tuba",
        "60 0 Muted Trumpet",
        "60 1 Muted Trumpet 2",
        "61 0 French Horns",
        "61 1 French Horn 2",
        "62 0 Brass Section 1",
        "62 1 Brass Section 2",
        "63 0 Synth Brass 1",
        "63 1 Synth Brass 3",
        "63 2 Analog Brass 1",
        "63 3 Jump Brass",
        "64 0 Synth Brass 2",
        "64 1 Synth Brass 4",
        "64 2 Analog Brass 2",
        "Reed",
        "65 0 Soprano Sax",
        "66 0 Alto Sax",
        "67 0 Tenor Sax",
        "68 0 Baritone Sax",
        "69 0 Oboe",
        "70 0 English Horn",
        "71 0 Bassoon",
        "72 0 Clarinet",
        "Wind",
        "73 0 Piccolo",
        "74 0 Flute",
        "75 0 Recorder",
        "76 0 Pan Flute",
        "77 0 Bottle Blow",
        "78 0 Shakuhachi",
        "79 0 Whistle",
        "80 0 Ocarina",
        "Synth Lead",
        "81 0 Square Lead",
        "81 1 Square Wave",
        "81 2 Sine Wave",
        "82 0 Saw Lead",
        "82 1 Saw Wave",
        "82 2 Doctor Solo",
        "82 3 Natural Lead",
        "82 4 Sequenced Saw",
        "83 0 Synth Calliope",
        "84 0 Chiffer Lead",
        "85 0 Charang",
        "85 1 Wire Lead",
        "86 0 Solo Synth vox",
        "87 0 5th Saw Wave",
        "88 0 Bass & Lead",
        "88 1 Delayed Lead",
        "Synth Pad",
        "89 0 Fantasia Pad",
        "90 0 Warm Pad",
        "90 1 Sine Pad",
        "91 0 Polysynth Pad",
        "92 0 Space Voice Pad",
        "92 1 Itopia",
        "93 0 Bowed Glass Pad",
        "94 0 Metal Pad",
        "95 0 Halo Pad",
        "96 0 Sweep Pad",
        "Synth Sound FX",
        "97 0 Ice Rain",
        "98 0 Soundtrack",
        "99 0 Crystal",
        "99 1 Synth Mallet",
        "100 0 Atmosphere",
        "101 0 Brightness",
        "102 0 Goblin",
        "103 0 Echo Drops",
        "103 1 Echo Bell",
        "103 2 Echo Pan",
        "104 0 Star Theme",
        "Ethnic",
        "105 0 Sitar",
        "105 1 Sitar 2",
        "106 0 Banjo",
        "107 0 Shamisen",
        "108 0 Koto",
        "108 1 Taisho Koto",
        "109 0 Kalimba",
        "110 0 Bagpipe",
        "111 0 Fiddle",
        "112 0 Shanai",
        "Percussive",
        "113 0 Tinkle Bell",
        "114 0 Agogo",
        "115 0 Steel Drums",
        "116 0 Woodblock",
        "116 1 Castanets",
        "117 0 Taiko",
        "117 1 Concert Bass Drums",
        "118 0 Melodic Tom 1",
        "118 1 Meoldic Tom 2",
        "119 0 Synth Drum",
        "119 1 808 Tom",
        "119 2 Electric Percussion",
        "120 0 Reverse Cymbal",
        "Sound Effect",
        "121 0 Guitar Fret Noise",
        "121 1 Guitar Cut Noise",
        "121 2 String Slap",
        "122 0 Breath Noise",
        "122 1 Flute Key Click",
        "123 0 Seashore",
        "123 1 Rain",
        "123 2 Thunder",
        "123 3 Wind",
        "123 4 Stream",
        "123 5 Bubble",
        "124 0 Bird",
        "124 1 Dog",
        "124 2 Horse-Gallop",
        "124 3 Bird 2",
        "125 0 Telephone 1",
        "125 1 Telephone 2",
        "125 2 Door Creaking",
        "125 3 Door Closing",
        "125 4 Scratch",
        "125 5 Wind Chimes",
        "126 0 Helicopter",
        "126 1 Car-Engine",
        "126 2 Car-Stop",
        "126 3 Car-Pass",
        "126 4 Car-Crash",
        "126 5 Siren",
        "126 6 Train",
        "126 7 Jetplane",
        "126 8 Starship",
        "126 9 Burst Noise",
        "127 0 Applause",
        "127 1 Laughing",
        "127 2 Screaming",
        "127 3 Punch",
        "127 4 Heart Beat",
        "127 5 Footsteps",
        "128 0 Gun Shot",
        "128 1 Machine Gun",
        "128 2 Lasergun",
        "128 3 Explosion"
    };
    
    return data;
}

QList<QString> MIDISoundsList::getNuanceVelocities()
{
    QList<QString> list;
    
    list << "008: pppp" << "020: ppp" << "031: pp" << "042: p" << "053: mp" << "064: mf" << "080: f" << "096: ff" << "112: fff" << "127: ffff";
    
    return list;
}
int MIDISoundsList::getVelocityForString(QString value)
{
    QList<QString> list_velocities = this->getNuanceVelocities();
    for (int i=0; i < list_velocities.length(); i++)
    {
        if (list_velocities.at(i) == value)
        {
            int velocity = list_velocities.at(i).split(":")[0].toInt();
            return velocity;
        }
    }
    
    // in case something went wrong, fall back to a default
    return 64;
}
QString MIDISoundsList::getStringForVelocity(int value)
{
    QList<QString> list_velocities = this->getNuanceVelocities();
    for (int i=0; i < list_velocities.length(); i++)
    {
        int velocity = list_velocities.at(i).split(":")[0].toInt();
        if (velocity == value)
        {
            return list_velocities.at(i);
        }
    }
    
    // in case something went wrong, fall back to a default
    return list_velocities.at(5);
}
