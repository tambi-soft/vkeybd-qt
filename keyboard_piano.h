#ifndef KEYBOARDPIANO_H
#define KEYBOARDPIANO_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QMap>
#include <QList>
#include <QPalette>

#include "button_piano.h"
#include "interface_audio.h"
#include "interface_alsa.h"
#include "interface_jack.h"

class KeyboardPiano : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardPiano(QWidget *parent = nullptr);
    
    QList<QString> notes_full = {"c", "d", "e", "f", "g", "a", "b"};
    QList<QString> notes_half = {"cis", "dis", "fis", "gis", "ais"};
    
    void setOctaves(int octaves);
    int octaves = 1;
    
    int button_width_full = 18;
    int button_height_full = 80;
    int button_width_half = 14;
    int button_height_half = 50;
    bool invert_colors = false;
    
    //JackAdapter *jack = new JackAdapter;
    InterfaceAudio *interface_audio;
    
    void drawOneOctave(int octave);
    
    QMap<QString, ButtonPiano*> list_of_keys;
    
    QSize size;
    
    void keyPressed(QString keycode);
    void keyReleased(QString keycode);
    
private:
    void colorizeBlackKeys(ButtonPiano *button);
    void colorizeWhiteKeys(ButtonPiano *button);
    
    
signals:
    
};

#endif // KEYBOARDPIANO_H
