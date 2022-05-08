#ifndef KEYBOARDPC_H
#define KEYBOARDPC_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QList>
#include <QDebug>

class KeyboardPC : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardPC(QWidget *parent = nullptr);
    
    float button_scale = .799;
    
    void keyDown(int keycode);
    void keyUp(int keycode);
    void keyDownRaw(int keycode);
    void keyUpRaw(int keycode);
    void allKeysUp();
    
private:
    QList<QList<int>> getButtonSizes();
    QList<QList<QString>> getButtonColors();
    QList<QList<QString>> getButtonLabels();
    // some keys can have different codes on up or down (e.g. Shift-Keys on German Neo2)
    QList<QList<int>> getButtonKeycodesDown();
    QList<QList<int>> getButtonKeycodesUp();
    QList<QList<int>> getButtonKeycodesRaw();
    QList<QList<int>> getButtonKeycodesNative();
    QList<QList<int>> getMIDICodes();
    
    QList<QList<QPushButton*>> list_of_buttons;
    
    void drawButtons();
    float calculateOffset(float row_offset, int row, int col, QList<QList<int>> sizes);
    
signals:
    void MIDIPress(int midicode);
    void MIDIRelease(int midicode);
};

#endif // KEYBOARDPC_H
