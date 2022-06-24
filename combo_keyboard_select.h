#ifndef COMBOKEYBOARDSELECT_H
#define COMBOKEYBOARDSELECT_H

#include <QWidget>
#include <QComboBox>
#include <QEvent>

#include "input_keyboard_raw.h"

// In vkeybd-qt, the keypresses are for playing your instrument. Therefore we want to have a QComboBox with deactivated search function. We acchieve that with this class and overwriting the eventFilter to ignore KeyPress events
class ComboKeyboardSelect : public QComboBox
{
    Q_OBJECT
public:
    ComboKeyboardSelect(QComboBox *parent = nullptr);
    
private:
    
    
protected:
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // COMBOKEYBOARDSELECT_H
