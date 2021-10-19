#ifndef INPUTKEYBOARDRAW_H
#define INPUTKEYBOARDRAW_H

#include <QObject>

class InputKeyboardRaw : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardRaw(QObject *parent = nullptr);
    
signals:
    
};

#endif // INPUTKEYBOARDRAW_H
