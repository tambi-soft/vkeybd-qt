#ifndef INPUTKEYBOARDRAWTHREAD_H
#define INPUTKEYBOARDRAWTHREAD_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class InputKeyboardRawThread : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardRawThread(QObject *parent = nullptr);
    
signals:
    void keyboardPressed(QString keyboard_id);
    
public:
    void start();
    
};

#endif // INPUTKEYBOARDRAWTHREAD_H
