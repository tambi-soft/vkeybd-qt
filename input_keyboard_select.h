#ifndef INPUTKEYBOARDSELECT_H
#define INPUTKEYBOARDSELECT_H

#include <QObject>
#include <QComboBox>
#include <QPushButton>
#include <QMap>
#include <QModelIndex>

#include <QDebug>

#include "enums_structs.h"
#include "input_keyboard_raw.h"
#include "combo_keyboard_select.h"
#include "enums_structs.h"

class InputKeyboardSelect : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardSelect(ComboKeyboardSelect *combo_keyboard_selector, QPushButton *button_keyboard_lock, QPushButton *button_keyboard_rescan, QObject *parent = nullptr);
    
    void keyboardRescan();
    bool isKeyboardLocked();
    
    void keyboardSelectionChanged(int index);
    
private:
    ComboKeyboardSelect *combo_keyboard_selector;
    QPushButton *button_keyboard_lock;
    QPushButton *button_keyboard_rescan;
    
    bool keyboard_locked = false;
    
    QMap<int,QString> combo_keyboard_input_labels;
    
    InputKeyboardRawMeta *keyboard_raw;
    QMap<QString, InputKeyboardRawController*> map_of_raw_keyboards;
    void instantiateRawKeyboards(QList<QString> keyboard_names);
    void cleanupRawKeyboards();
    
    void disconnectRawKeyboards();
    
    bool locked = false;
    void toggleKeyboardLock();
    
    void autoSelectPressedKeyboard(QString name);
    void rawKeyboardSelected(int index);
    
    int display_input_type;
    void detectDisplayInput();
    
private slots:
    void keyRawPressed(int keycode);
    void keyRawReleased(int keycode);
    
signals:
    /*
    void keyboardSelectedNone();
    void keyboardSelectedNative();
    void KeyboardSeletedDefaul();
    */
    void keyboardSelectionChangedSignal(int selection, bool locked);
    
    void keyRawPressedSignal(int keycode);
    void keyRawReleasedSignal(int keycode);
};

#endif // INPUTKEYBOARDSELECT_H
