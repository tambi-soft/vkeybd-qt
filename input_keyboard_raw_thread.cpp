#include "input_keyboard_raw_thread.h"

// https://stackoverflow.com/questions/26373963/distinguish-2-keyboards-keystrokes-using-eventfilter-embedded-linux

InputKeyboardRawThread::InputKeyboardRawThread(QObject *parent) : QObject(parent)
{
    start();
}

void InputKeyboardRawThread::start()
{
    QString path = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    //QString path = "/dev/input/by-id/FileForKeyboard1";
    QFile file(path);
    
    if(file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QTextStream stream( &file );
        
        /*
        while(true)
        {
            //stream.read(1);
            //emit keyBoard1_Pressed();
            //emit keyboardPressed("bla");
        }
        */
        
        qDebug() << "juhuu";
        
        while (true)
        {
            qDebug() << stream.read(1);
            
        }
        
        
        
    }
}
