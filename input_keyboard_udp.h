#ifndef INPUTKEYBOARDUDP_H
#define INPUTKEYBOARDUDP_H

#include <QObject>

class InputKeyboardUDP : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardUDP(QObject *parent = nullptr);
    
signals:
    
};

#endif // INPUTKEYBOARDUDP_H
