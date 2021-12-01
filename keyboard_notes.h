#ifndef KEYBOARDNOTES_H
#define KEYBOARDNOTES_H

#include <QObject>
#include <QWidget>

class KeyboardNotes : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardNotes(QWidget *parent = nullptr);
    
    void keyPressed(int midicode);
    void keyReleased(int midicode);
    void allKeysUp();
    
signals:
    
};

#endif // KEYBOARDNOTES_H
