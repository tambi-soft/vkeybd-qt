#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

enum class MIDISignalTypes
{
    KeyPress = 0,
    KeyRelease = 1,
    ToggleKeyboardLock = 2,
    Panick = 3,
    StopAll = 4,
    ShowMenu = 5,
    ResendMIDISettings = 6,
    
    SustainPressed = 7,
    SustainReleased = 8,
    
    SostenutoPressed = 9,
    SostenutoReleased = 10,
    
    SoftPressed = 11,
    SoftReleased = 12,
    
    VolumeLowerPressed = 13,
    VolumeHigherPressed = 14,
    VolumeReleased = 15,
    
    PitchLowerPressed = 16,
    PitchHigherPressed = 17,
    PitchReleased = 18,
};

enum class TabNames
{
    F1 = 0,
    F2 = 1,
    F3 = 2,
    F4 = 3,
    
    F5 = 4,
    F6 = 5,
    F7 = 6,
    F8 = 7,
    
    F9 = 8,
    F10 = 9,
    F11 = 10,
    F12 = 11
};

#endif // ENUMS_H
