#ifndef INPUTKEYBOARDRAWTHREAD_H
#define INPUTKEYBOARDRAWTHREAD_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class InputKeyboardRawThread : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardRawThread(QObject *parent = nullptr);
    
signals:
    
};

#endif // INPUTKEYBOARDRAWTHREAD_H
