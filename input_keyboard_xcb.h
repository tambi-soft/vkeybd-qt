#ifndef INPUTKEYBOARDXCB_H
#define INPUTKEYBOARDXCB_H

#include "enums.h"

#include "qmetatype.h"
#include "qtextstream.h"

#include <xcb/xcb.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
// https://interest.qt-project.narkive.com/h3IP384b/compiler-error-from-moc-mainwindow-cpp
#undef Bool

#include <QObject>
#include <QX11Info> 
#include <QMap>
#include <QDebug>

class InputKeyboardXCB : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardXCB(QObject *parent = nullptr);
    
    bool xcbEvent(const QByteArray &eventType, void *message, long *result);
    
private:
    bool isAutoRepeat(xcb_generic_event_t* xev, void *message);
    
    QList<int> list_of_keypresses;
    
signals:
    void rawKeyPressedSignal(int key);
    void rawKeyReleasedSignal(int key);
    
    void toggleKeyboardLockSignal();
    void changeTabSignal(int id);
    void MIDISignal(MIDISignalTypes type);
};

#endif // INPUTKEYBOARDXCB_H
