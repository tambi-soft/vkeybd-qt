#ifndef COMBOKEYBOARDSELECT_H
#define COMBOKEYBOARDSELECT_H

#include <QWidget>
#include <QComboBox>
#include <QEvent>

#include "input_keyboard_raw.h"

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
