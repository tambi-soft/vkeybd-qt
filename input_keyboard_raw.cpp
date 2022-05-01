#include "input_keyboard_raw.h"

InputKeyboardRaw::InputKeyboardRaw(QObject *parent)
    : QObject{parent}
{
    detectKeyboards();
    //getKeyboardNames();
    //readKeyboardEvents();
}

QList<QMap<QString,QString>> InputKeyboardRaw::detectKeyboards()
{
    QList<QMap<QString,QString>> keyboards;
    
    QFile file("/proc/bus/input/devices");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        QString devs = stream.readAll();
        QList<QString> lines = devs.split("\n");
        
        QMap<QString,QString> device;
        for (int i=0; i < lines.length(); i++)
        {
            QString line = lines.at(i);
            
            if (line.isEmpty())
            {
                // https://unix.stackexchange.com/questions/74903/explain-ev-in-proc-bus-input-devices-data
                if (device["EV"] == "120013")
                {
                    keyboards.append(device);
                }
                device.clear();
            }
            else
            {
                QList<QString> splitted = line.split(": ").at(1).split("=");
                if (splitted.length() > 1)
                {
                    if (splitted.at(0) == "Handlers")
                    {
                        QList<QString> hand = splitted.at(1).split(" ");
                        for (int j=0; j < hand.length(); j++)
                        {
                            if (hand.at(j).startsWith("event"))
                            {
                                device["devpath"] = "/dev/input/"+hand.at(j);
                            }
                        }
                    }
                    QString key = splitted.at(0);
                    QString value = splitted.at(1);
                    device[key] = value.remove("\"");
                }
            }
        }
    }
    file.close();
    
    return keyboards;
}
QList<QString> InputKeyboardRaw::getKeyboardNames()
{
    QList<QMap<QString,QString>> keyboards = detectKeyboards();
    
    QList<QString> result;
    for (int i=0; i < keyboards.length(); i++)
    {
        result.append(keyboards.at(i)["Name"]);
    }
    
    return result;
}
QString InputKeyboardRaw::getPathForName(QString name)
{
    QList<QMap<QString,QString>> keyboards = detectKeyboards();
    
    for (int i=0; i < keyboards.length(); i++)
    {
        if (keyboards.at(i)["Name"] == name)
        {
            return keyboards.at(i)["devpath"];
        }
    }
    
    return "";
}

void InputKeyboardRaw::keyboardListen(QString devpath)
{
    /*
    QFile file(devpath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Binary))
    {
        QDataStream stream(&file);
        qDebug() << stream.readRawData();
    }
    */
}
void InputKeyboardRaw::keyboardLock(QString devpath)
{
    // https://stackoverflow.com/questions/29942421/read-barcodes-from-input-event-linux-c/29956584#29956584
    // https://www.reddit.com/r/Cplusplus/comments/rsgjwf/ioctl_in_c_c_wrapper_class_for_linuxjoystickh/
    
    char *dev = devpath.toLocal8Bit().data();
    if ((this->fd = open(dev, O_RDONLY)) >= 0)
    {
        errno = 0;
        // to consume the event and not let it passed through to any other software
        if (ioctl(this->fd, EVIOCGRAB, 1)) {
            const int saved_errno = errno;
            close(this->fd);
            //return errno = (saved_errno) ? errno : EACCES;
        }
        
        // https://stackoverflow.com/questions/20943322/accessing-keys-from-linux-input-device
        while (true)
        {
            n = read(this->fd, &this->ev, sizeof this->ev);
            
            if (n == (ssize_t)-1)
            {
                continue;
            }
            else if (n != sizeof ev)
            {
                continue;
            }
            
            if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2)
            {
                //printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
                if (ev.value == 1)
                {
                    emit rawKeyPressed(ev.code);
                }
                else if (ev.value == 0)
                {
                    emit rawKeyReleased(ev.code);
                }
            }
        }
    }
    else
    {
        emit deviceNotAvailable("evdev open");
    }
}

void InputKeyboardRaw::keyboardRelease()
{
    
}
