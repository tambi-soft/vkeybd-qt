#ifndef INPUTKEYBOARDXCB_H
#define INPUTKEYBOARDXCB_H

#include "qmetatype.h"
#include "qtextstream.h"

#include <xcb/xcb.h>
//extern "C"
//{
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#undef Bool
//}

#include <QObject>
#include <QX11Info> 
#include <QMap>
#include <QDebug>

//#include <QDebug>
//#include <QX11Info>
//#include <xcb/xcb.h>
//#include <X11/Xlib.h>

//#include <X11/extensions/XInput2.h>
//#include <X11/X.h>


//#include <X11/Xutil.h>
//#include <X11/Xlib.h>
//#include <X11/XKBlib.h>

//#include <stdbool.h>

class InputKeyboardXCB : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardXCB(QObject *parent = nullptr);
    
    bool xcbEvent(const QByteArray &eventType, void *message, long *result);
    
private:
    bool isAutoRepeat(xcb_generic_event_t* xev, void *message);
    
    //QMap<int, int> map_of_keypresses;
    QList<int> list_of_keypresses;
    
signals:
    void rawKeyPressedSignal(int key);
    void rawKeyReleasedSignal(int key);
};

#endif // INPUTKEYBOARDXCB_H
