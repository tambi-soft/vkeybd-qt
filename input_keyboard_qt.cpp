#include "input_keyboard_qt.h"

InputKeyboardQt::InputKeyboardQt(QObject *parent)
    : QObject{parent}
{
    qDebug() << QLocale::system();
    //qDebug() << QLocale::Q
    
}

bool InputKeyboardQt::callEventFilter(QObject *obj, QEvent *ev)
{
    Q_UNUSED(obj);
    
    // as this object does not have focus when this function is called, Qt does not count this as a KeyPress but a ShortcutOverride instead
    if (ev->type() == QEvent::KeyPress || ev->type() == QEvent::ShortcutOverride)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(ev);
        bool is_auto_repeat = isAutoRepeat(ev, event);
        if (!is_auto_repeat)
        {
            int key = event->nativeScanCode() -8;
            emit keyPressSignal(key);
            
            return true;
        }
    }
    else if (ev->type() == QEvent::KeyRelease)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(ev);
        bool is_auto_repeat = isAutoRepeat(ev, event);
        if (!is_auto_repeat)
        {
            int key = event->nativeScanCode() -8;
            emit keyReleaseSignal(key);
            
            return true;
        }
    }
    
    return false;
}

bool InputKeyboardQt::isAutoRepeat(QEvent *event, QKeyEvent *key)
{
    bool autorepeat_detected = false;
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::ShortcutOverride)
    {
        if (this->list_of_keypresses.contains(key->key()))
        {
            autorepeat_detected = true;
        }
        else
        {
            this->list_of_keypresses.append(key->key());
        }
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        int index = this->list_of_keypresses.indexOf(key->key());
        if (index >= 0)
        {
            this->list_of_keypresses.removeAt(index);
        }
    }
    
    return autorepeat_detected;
}

int InputKeyboardQt::mapFromKeyboardLayoutToLinuxRaw(int key)
{
    QMap<int, int> map_layout;
    
    QList<int> list_qt;
    list_qt = {Qt::Key_F1, Qt::Key_F2, Qt::Key_F3, Qt::Key_F4,
               Qt::Key_F5, Qt::Key_F6, Qt::Key_F7, Qt::Key_F8,
               Qt::Key_F9, Qt::Key_F10, Qt::Key_F11, Qt::Key_F12,
              Qt::Key_Up, Qt::Key_Down, Qt::Key_PageUp, Qt::Key_PageDown, Qt::Key_Left, Qt::Key_Right, Qt::Key_Escape, Qt::Key_Delete, Qt::Key_Insert, Qt::Key_Menu, Qt::Key_Space, Qt::Key_Alt, Qt::Key_Super_L, Qt::Key_Super_R,
               16781906, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 16781904, 16777219,
               16777217, 88, 86, 76, 67, 87, 75, 72, 71, 70, 81, 223, 16781905, 16777220,
               16781571, 85, 73, 65, 69, 79, 83, 78, 82, 84, 68, 89, 16781571,
               16777252, 0, 220, 214, 196, 80, 90, 66, 77, 44, 46, 74, 16777248,
               -1, 16777299, 16777251, 32, 16777251, -1, -1, -1, -1, -1, -1};
    
    QList<int> list_raw;
    list_raw = {KeysRaw::F1, KeysRaw::F2, KeysRaw::F3, KeysRaw::F4,
                KeysRaw::F5, KeysRaw::F6, KeysRaw::F7, KeysRaw::F8,
                KeysRaw::F9, KeysRaw::F10, KeysRaw::F11, KeysRaw::F12,
               KeysRaw::Up, KeysRaw::Down, KeysRaw::PageUp, KeysRaw::PageDown, KeysRaw::Left, KeysRaw::Right, KeysRaw::Escape, KeysRaw::Delete, KeysRaw::Insert, KeysRaw::Menu, KeysRaw::Space, KeysRaw::Alt, KeysRaw::Super_L, KeysRaw::Super_R,
                41, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                58, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 43,
                42, 86, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
                -1, 125, 56, 57, 100, -1, -1, -1, -1, -1, -1};
    
    if (list_qt.length() == list_raw.length())
    {
        for (int i=0; i < list_raw.length(); i++)
        {
            map_layout[list_qt.at(i)] = list_raw.at(i);
        }
    }
    
    return map_layout[key];
}

void InputKeyboardQt::sendUDPMessage(QString message)
{
    
}
