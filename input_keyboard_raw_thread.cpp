#include "input_keyboard_raw_thread.h"

// https://stackoverflow.com/questions/26373963/distinguish-2-keyboards-keystrokes-using-eventfilter-embedded-linux

InputKeyboardRawThread::InputKeyboardRawThread(QObject *parent) : QObject(parent)
{
    QFile file("/dev/input/by-id/FileForKeyboard1");
    
    if(file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QTextStream stream( &file );
        while(true)
        {
            stream.read(1);
            //emit keyBoard1_Pressed();
        }
    }
}
