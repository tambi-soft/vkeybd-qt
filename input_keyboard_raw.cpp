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
            //qDebug() << line;
            
            if (line.isEmpty())
            {
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

/*
void InputKeyboardRaw::getKeyboardNames()
{
    char name[256]= "Unknown";
    
    this->dev = "/dev/input/by-id/usb-Cherry_USB_keyboard-event-kbd";
    //ssthis->dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    if ((this->fd = open(this->dev, O_RDONLY)) < 0) {
        perror("evdev open");
        exit(1);
    }
    
    if(ioctl(this->fd, EVIOCGNAME(sizeof(name)), name) < 0) {
        perror("evdev ioctl");
    }
    
    //printf("The device on %s says its name is %s\n",
    //    device, name);
    
    qDebug() << name;
    
    //close(this->fd);
}
*/

void InputKeyboardRaw::openInputDevice()
{
    //const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    //struct input_event ev;
    //ssize_t n;
    //int fd;

    this->fd = open(this->dev, O_RDONLY);
    if (this->fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", this->dev, strerror(errno));
        //return EXIT_FAILURE;
    }
}

void InputKeyboardRaw::readKeyboardEvents()
{
    while (true) {
        n = read(this->fd, &this->ev, sizeof this->ev);
        /*
        if (n == (ssize_t)-1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else
        if (n != sizeof ev) {
            errno = EIO;
            break;
        }
        */
        
        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2)
            printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
    }
}

void InputKeyboardRaw::listenOnKeyboard(QString devpath)
{
    char *dev = devpath.toLocal8Bit().data();
    if ((this->fd = open(dev, O_RDONLY)) >= 0)
    {
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
                printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
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
void InputKeyboardRaw::lockOnKeyboard(QString devpath)
{
    
}
