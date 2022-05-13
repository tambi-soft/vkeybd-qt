#ifndef INPUTKEYBOARDQT_H
#define INPUTKEYBOARDQT_H

#include <QObject>

class InputKeyboardQt : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardQt(QObject *parent = nullptr);
    
signals:
    
};

#endif // INPUTKEYBOARDQT_H
