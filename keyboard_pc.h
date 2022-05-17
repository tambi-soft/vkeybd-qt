#ifndef KEYBOARDPC_H
#define KEYBOARDPC_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QList>
#include <QDebug>

#include "enums.h"

class KeyboardPC : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardPC(QWidget *parent = nullptr);
    
    float button_scale = .799;
    
    void keyDownRaw(int keycode);
    void keyUpRaw(int keycode);
    void allKeysUp();
    
private:
    QList<QList<int>> getButtonSizes();
    QList<QList<QString>> getButtonColors();
    QList<QList<QString>> getButtonLabels();
    // "raw" means: as read directly from /dev/input
    QList<QList<int>> getButtonKeycodesRaw();
    QList<QList<int>> getMIDICodes();
    
    QList<QList<QPushButton*>> list_of_buttons;
    
    void drawButtons();
    float calculateOffset(float row_offset, int row, int col, QList<QList<int>> sizes);
    
signals:
    void MIDIPress(int midicode);
    void MIDIRelease(int midicode);
};

#endif // KEYBOARDPC_H
