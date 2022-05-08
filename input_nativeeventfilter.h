#ifndef INPUTNATIVEEVENTFILTER_H
#define INPUTNATIVEEVENTFILTER_H

#include <QAbstractNativeEventFilter>
#include <QByteArray>
#include <QDebug>

#include <xcb/xcb.h>
//#include <X11/Xlib.h>

class InputNativeEventFilter : public QAbstractNativeEventFilter
{
public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override
    {
        Q_UNUSED(result);
        
        qDebug() << "native event";
        
        if (eventType == "xcb_generic_event_t") {
            xcb_generic_event_t* xev = static_cast<xcb_generic_event_t *>(message);
            
            qDebug() << xev;
            
            /*
            if ((xev->response_type & ~0x80) == XCB_KEY_PRESS) {
                  xcb_key_press_event_t* kp = (xcb_key_press_event_t*)xev;
            
                  const quint32 keycode = kp->detail;
                  const quint32 keymod =
                      static_cast<quint32>(kp->state & (ShiftMask | ControlMask |
                                                        Mod1Mask | Mod4Mask));
             }
             */
        }
        return false;
    }
};

#endif // INPUTNATIVEEVENTFILTER_H
