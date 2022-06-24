#ifndef INPUTKEYBOARDRAW_H
#define INPUTKEYBOARDRAW_H

// https://unix.stackexchange.com/questions/72483/how-to-distinguish-input-from-different-keyboards
// https://www.linuxjournal.com/files/linuxjournal.com/linuxjournal/articles/064/6429/6429l4.html
// https://stackoverflow.com/questions/26373963/distinguish-2-keyboards-keystrokes-using-eventfilter-embedded-linux

#include <linux/input.h>

#include <QObject>
#include <QTimer>

class InputKeyboardRawWorker : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardRawWorker(ssize_t n, int fd, QObject *parent = nullptr);
    //~InputKeyboardRawWorker();
    
    void finish();
    
private:
    QTimer *timer = nullptr;
    
    struct input_event ev;
    ssize_t n;
    int fd = -1;
    
public slots:
    // https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
    // "one extremely important thing to note here is that you should NEVER allocate heap objects (using new) in the constructor of the QObject class as this allocation is then performed on the main thread and not on the new QThread instance, meaning that the newly created object is then owned by the main thread and not the QThread instance. This will make your code fail to work."
    void initialize();
    
    void tick();
    
signals:
    void finished();
    
    void rawKeyPressed(int code);
    void rawKeyReleased(int code);
};



class InputKeyboardRawMeta : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardRawMeta(QObject *parent = nullptr);
    
    QList<QMap<QString, QString> > detectKeyboards();
    QList<QString> getKeyboardNames();
    QString getPathForName(QString name);
    
private:
    // in case we have two identical keyboards attached, it is helpful to see the /dev/input/eventXX - eventname as part of the device name
    QString getKeyboardName(QMap<QString, QString> keyboard);
};



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
#include <QThread>
#include <QMetaObject>
#include <QDebug>

class InputKeyboardRawController : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardRawController(QObject *parent = nullptr);
    ~InputKeyboardRawController();
    
    void keyboardListen(QString devpath);
    void keyboardLock(QString devpath);
    void keyboardHelper(QString devpath, QString mode);
    void keyboardRelease();
    
private:
    // https://stackoverflow.com/questions/16695432/input-event-structure-description-from-linux-input-h
    const char *const evval[3] = {
        "RELEASED",
        "PRESSED ",
        "REPEATED"
    };
    
    const char *dev;
    struct input_event ev;
    ssize_t n;
    int fd = -1;
    
    QThread *thread = nullptr;
    InputKeyboardRawWorker *worker;
    
private slots:
    void rawKeyPressed(int keycode);
    void rawKeyReleased(int keycode);
    
signals:
    void deviceNotAvailable(QString message);
    void rawKeyPressedSignal(int keycode);
    void rawKeyReleasedSignal(int keycode);
};

#endif // INPUTKEYBOARDRAW_H
