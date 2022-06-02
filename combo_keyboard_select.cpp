#include "combo_keyboard_select.h"

ComboKeyboardSelect::ComboKeyboardSelect(QComboBox *parent) : QComboBox(parent)
{
    installEventFilter(this);
}

bool ComboKeyboardSelect::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object);
    
    if (event->type() == QEvent::KeyPress) {
        return true;
    }
    return false;
}
