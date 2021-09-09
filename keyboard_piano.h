#ifndef KEYBOARDPIANO_H
#define KEYBOARDPIANO_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

class KeyboardPiano : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardPiano(QWidget *parent = nullptr);
    
signals:
    
};

#endif // KEYBOARDPIANO_H
