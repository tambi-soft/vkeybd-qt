#ifndef KEYBOARDPIANO_H
#define KEYBOARDPIANO_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMap>

class KeyboardPiano : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardPiano(QWidget *parent = nullptr);
    void setOctaves(int octaves);
    int octaves = 1;
    int button_width_full = 18;
    int button_height_full = 80;
    int button_width_half = 15;
    int button_height_half = 60;
    void drawOneOctave(int offset);
    
    QMap<QString, QPushButton*> list_of_keys;
    
private:
    
    
signals:
    
};

#endif // KEYBOARDPIANO_H
