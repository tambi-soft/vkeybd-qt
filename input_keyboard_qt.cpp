#include "input_keyboard_qt.h"

InputKeyboardQt::InputKeyboardQt(QObject *parent)
    : QObject{parent}
{
    qDebug() << QLocale::system();
    //qDebug() << QLocale::Q
    
    //this->list_function_keys = {Qt::Key_F1, Qt::Key_F2, Qt::Key_F3, Qt::Key_F4, -1, Qt::Key_F5, Qt::Key_F6, Qt::Key_F7, Qt::Key_F8, -1, Qt::Key_F9, Qt::Key_F10, Qt::Key_F11, Qt::Key_F12};
}

bool InputKeyboardQt::callEventFilter(QObject *obj, QEvent *ev)
{
    Q_UNUSED(obj);
    
    QKeyEvent *event = static_cast<QKeyEvent*>(ev);
    bool is_auto_repeat = isAutoRepeat(ev, event);
    
    if (ev->type() == QEvent::KeyPress)
    {
        if (!is_auto_repeat)
        //if (!event->isAutoRepeat())       
        {
            qDebug() << "EEEEEEEEEEEEEEEEEEEEEEEE";
            if (event->key() == Qt::Key_Control)
            {
                this->ctrl_down = true;
            }
            else if (event->key() == Qt::Key_Shift)
            {
                if (this->ctrl_down)
                {
                    qDebug() << "ctr shift";
                    //toggleKeyboardLock();
                    emit toggleKeyboardLockSignal();
                    
                    return true;
                }
            }
            
            //Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget()->layout()->itemAt(0)->widget());
            
            QString udp_message = QString::number(ev->type())+"/"+QString::number(event->key());
            
            if (event->key() == Qt::Key_Escape)
            {
                //emit panickSignal();
                emit MIDISignal(MIDISignalTypes::Panick);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Delete)
            {
                //emit stopAllSignal();
                emit MIDISignal(MIDISignalTypes::StopAll);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Menu)
            {
                //emit showMenuSignal();
                emit MIDISignal(MIDISignalTypes::ShowMenu);
                
                sendUDPMessage(udp_message);
            }
            else if (event->key() == Qt::Key_Insert)
            {
                //emit resendMIDISettingsSignal();
                emit MIDISignal(MIDISignalTypes::ResendMIDISettings);
                
                sendUDPMessage(udp_message);
            }
            else if (event->key() == Qt::Key_Space)
            {
                emit MIDISignal(MIDISignalTypes::SustainPressed);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Alt)
            {
                emit MIDISignal(MIDISignalTypes::SostenutoPressed);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Super_L || event->key() == Qt::Key_Super_R)
            {
                emit MIDISignal(MIDISignalTypes::SoftPressed);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            // activate desired tab by pressing an f-key
            /*
            else if (this->list_function_keys.contains(event->key()))
            {
                for (int i=0; i < this->list_function_keys.length(); i++)
                {
                    if (this->list_function_keys.at(i) == event->key())
                    {
                        // activate new tab
                        emit changeTabSignal(i);
                        
                        sendUDPMessage(udp_message);
                        
                        return true;
                    }
                }
            }
            */
            // use the arrow-keys to operate the pitch-wheel
            else if (event->key() == Qt::Key_Left)
            {
                emit MIDISignal(MIDISignalTypes::PitchLowerPressed);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Right)
            {
                emit MIDISignal(MIDISignalTypes::PitchHigherPressed);
            }
            //else if (event->key() == Qt::Key_Up | event->key() == Qt::Key_Down | event->key() == Qt::Key_PageUp | event->key() == Qt::Key_PageDown)
            else if (event->key() == Qt::Key_Down)
            {
                emit MIDISignal(MIDISignalTypes::VolumeLowerPressed);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Up)
            {
                emit MIDISignal(MIDISignalTypes::VolumeHigherPressed);
            }
            // input for the virtual keyboard(s)
            else
            {
                int key = mapFromKeyboardLayoutToLinuxRaw(event->key());
                emit keyPressSignal(key);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
        }
    }
    else if (ev->type() == QEvent::KeyRelease)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(ev);
        
        if (!event->isAutoRepeat())
        {
            QString udp_message = QString::number(ev->type())+"/"+QString::number(event->key());
            
            if (event->key() == Qt::Key_Space)
            {
                emit MIDISignal(MIDISignalTypes::SustainReleased);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Alt)
            {
                emit MIDISignal(MIDISignalTypes::SostenutoReleased);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Super_L || event->key() == Qt::Key_Super_R)
            {
                emit MIDISignal(MIDISignalTypes::SoftReleased);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Left | event->key() == Qt::Key_Right)
            {
                emit MIDISignal(MIDISignalTypes::PitchReleased);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Up | event->key() == Qt::Key_Down | event->key() == Qt::Key_PageUp | event->key() == Qt::Key_PageDown)
            {
                emit MIDISignal(MIDISignalTypes::VolumeReleased);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            // filter out tab-keys from sending midi signals
            /*
            else if (this->list_function_keys.contains(event->key()))
            {
                return true;
            }
            */
            else
            {
                // this function should be kept just in case "areKeysPressed" would return "false" falsely on current tab ...
                //o->keyUp(event->key());
                // we want to have a smooth way of switching between tabs (=presets) during playing
                
                
                /*
                for (int i=0; i < this->list_of_tabs.length(); i++)
                {
                    //if (this->list_of_tabs.at(i)->areKeysPressed())
                    //{
                        this->list_of_tabs.at(i)->keyUp(event->key());
                    //}
                }
                */
                int key = mapFromKeyboardLayoutToLinuxRaw(event->key());
                emit keyReleaseSignal(key);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
        }
    }
    else if (ev->type() == QEvent::MouseButtonPress)
    {
        /*
        if (this->keyboard_locked)
        {
            emit toggleKeyboardLockSignal();
        }
        */
    }
    
    return false;
}

bool InputKeyboardQt::isAutoRepeat(QEvent *event, QKeyEvent *key)
{
    bool autorepeat_detected = false;
    if (event->type() == QEvent::KeyPress)
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
            16781906, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 16781904, 16777219,
            16777217, 88, 86, 76, 67, 87, 75, 72, 71, 70, 81, 223, 16781905, 16777220,
            16781571, 85, 73, 65, 69, 79, 83, 78, 82, 84, 68, 89, 16781571,
            16777252, 0, 220, 214, 196, 80, 90, 66, 77, 44, 46, 74, 16777248,
            -1, 16777299, 16777251, 32, 16777251, -1, -1, -1, -1, -1, -1};
    QList<int> list_raw;
    list_raw = {KeysRaw::F1, KeysRaw::F2, KeysRaw::F3, KeysRaw::F4,
                KeysRaw::F5, KeysRaw::F6, KeysRaw::F7, KeysRaw::F8,
                KeysRaw::F9, KeysRaw::F10, KeysRaw::F11, KeysRaw::F12,
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
