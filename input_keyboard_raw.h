#ifndef INPUTKEYBOARDRAW_H
#define INPUTKEYBOARDRAW_H

// https://unix.stackexchange.com/questions/72483/how-to-distinguish-input-from-different-keyboards
// https://www.linuxjournal.com/files/linuxjournal.com/linuxjournal/articles/064/6429/6429l4.html

/*
#include <sys/types.h>
#include <linux/hdreg.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
*/
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QDebug>

class InputKeyboardRaw : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardRaw(QObject *parent = nullptr);
    
    QList<QMap<QString, QString> > detectKeyboards();
    QList<QString> getKeyboardNames();
    QString getPathForName(QString name);
    void keyboardListen(QString devpath);
    void keyboardLock(QString devpath);
    void keyboardRelease();
    
private:
    // https://stackoverflow.com/questions/16695432/input-event-structure-description-from-linux-input-h
    const char *const evval[3] = {
        "RELEASED",
        "PRESSED ",
        "REPEATED"
    };
    
    const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    struct input_event ev;
    ssize_t n;
    int fd = -1;
    
signals:
    void deviceNotAvailable(QString message);
    void rawKeyPressed(int keycode);
    void rawKeyReleased(int keycode);
};

#endif // INPUTKEYBOARDRAW_H
