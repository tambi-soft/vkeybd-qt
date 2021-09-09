#ifndef KEYBOARDPIANO_H
#define KEYBOARDPIANO_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QMap>
#include <QPalette>

#include "button_piano.h"

class KeyboardPiano : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardPiano(QWidget *parent = nullptr);
    void setOctaves(int octaves);
    int octaves = 1;
    int button_width_full = 18;
    int button_height_full = 80;
    int button_width_half = 14;
    int button_height_half = 50;
    bool invert_colors = false;
    void drawOneOctave(int offset);
    
    QMap<QString, QPushButton*> list_of_keys;
    
    QSize size;
    
private:
    void colorizeBlackKeys(QPushButton *button);
    void colorizeWhiteKeys(QPushButton *button);
    
    
signals:
    
};

#endif // KEYBOARDPIANO_H
