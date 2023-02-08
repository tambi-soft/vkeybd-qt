#include "input_keyboard_select.h"

InputKeyboardSelect::InputKeyboardSelect(ComboKeyboardSelect *combo_keyboard_selector, QPushButton *button_keyboard_lock, QPushButton *button_keyboard_rescan, QObject *parent)
    : QObject{parent}
{
    this->combo_keyboard_selector = combo_keyboard_selector;
    this->button_keyboard_lock = button_keyboard_lock;
    this->button_keyboard_rescan = button_keyboard_rescan;
    
    connect(this->combo_keyboard_selector, &QComboBox::currentIndexChanged, this, &InputKeyboardSelect::keyboardSelectionChanged);
    
    connect(button_keyboard_rescan, &QPushButton::clicked, this, &InputKeyboardSelect::keyboardRescan);
    
    connect(this->button_keyboard_lock, &QPushButton::clicked, this, &InputKeyboardSelect::toggleKeyboardLock);
}

void InputKeyboardSelect::keyboardSelectionChanged(int index)
{
    if (index == KeyboardSelection::None)
    {
        this->button_keyboard_lock->setDisabled(true);
        disconnectRawKeyboards();
        
        emit keyboardSelectionChangedSignal(index, false);
    }
    else if (index == KeyboardSelection::Native)
    {
        this->button_keyboard_lock->setDisabled(false);
        disconnectRawKeyboards();
        
        emit keyboardSelectionChangedSignal(index, this->locked);
    }
    else if (index == KeyboardSelection::Default)
    {
        this->button_keyboard_lock->setDisabled(false);
        disconnectRawKeyboards();
        
        emit keyboardSelectionChangedSignal(index, this->locked);
    }
    else if (index == KeyboardSelection::Detect)
    {
        this->button_keyboard_lock->setDisabled(true);
        disconnectRawKeyboards();
        
        emit keyboardSelectionChangedSignal(index, false);
        
        qDebug() << this->map_of_raw_keyboards;
        QList<QString> keys = this->map_of_raw_keyboards.keys();
        for (int i=0; i < keys.length(); i++)
        {
            QString name = keys.at(i);
            QString path = this->keyboard_raw->getPathForName(name);
            InputKeyboardRawController *keyboard = this->map_of_raw_keyboards[name];
            keyboard->keyboardListen(path);
            
            qDebug() << name;
            connect(keyboard, &InputKeyboardRawController::signalRawKeyPressed, this, [this, name]{ this->autoSelectPressedKeyboard(name); });
        }
    }
    // some RAW-Keyboard selected
    else
    {
        rawKeyboardSelected(index);
    }
}
void InputKeyboardSelect::toggleKeyboardLock()
{
    if (this->locked)
    {
        this->locked = false;
        this->combo_keyboard_selector->setEnabled(true);
        this->button_keyboard_lock->setText("Lock");
        
        this->button_keyboard_rescan->setEnabled(true);
    }
    else
    {
        this->locked = true;
        this->combo_keyboard_selector->setEnabled(false);
        this->button_keyboard_lock->setText("Unlock");
        
        this->button_keyboard_rescan->setEnabled(false);
    }
    
    int index = this->combo_keyboard_selector->currentIndex();
    keyboardSelectionChanged(index);
}

void InputKeyboardSelect::disconnectRawKeyboards()
{
    QList<QString> keys = this->map_of_raw_keyboards.keys();
    for (int i=0; i < keys.length(); i++)
    {
        this->map_of_raw_keyboards[keys.at(i)]->keyboardRelease();
        this->map_of_raw_keyboards[keys.at(i)]->disconnect();
    }
}

void InputKeyboardSelect::autoSelectPressedKeyboard(QString name)
{
    QList<QString> keys = this->map_of_raw_keyboards.keys();
    for (int i=0; i < keys.length(); i++)
    {
        InputKeyboardRawController *kbd = this->map_of_raw_keyboards[keys.at(i)];
        
        kbd->disconnect();
        //kbd->deleteLater();
    }
    
    this->combo_keyboard_selector->setCurrentText(name);
    
    /*
    int index = this->combo_keyboard_selector->currentIndex();
    keyboardSelectionChanged(index);
    
    QList<QString> keys = this->map_of_raw_keyboards.keys();
    for (int i=0; i < keys.length(); i++)
    {
        this->map_of_raw_keyboards[keys.at(i)]->disconnect();
    }
    */
    
    //rawKeyboardSelected();
}

void InputKeyboardSelect::rawKeyboardSelected(int index)
{
    qDebug() << "RAW SELECTED";
    this->button_keyboard_lock->setDisabled(false);
    emit keyboardSelectionChangedSignal(index, this->locked);
    
    disconnectRawKeyboards();
    
    QString name = this->combo_keyboard_selector->currentText();
    QString path = this->keyboard_raw->getPathForName(name);
    
    QList<QString> keys = this->map_of_raw_keyboards.keys();
    for (int i=0; i < keys.length(); i++)
    {
        if (name == keys.at(i))
        {
            if (this->locked)
            {
                this->map_of_raw_keyboards[name]->keyboardLock(path);
            }
            else
            {
                this->map_of_raw_keyboards[name]->keyboardListen(path);
            }
            
            connect(this->map_of_raw_keyboards[name], &InputKeyboardRawController::signalRawKeyPressed, this, &InputKeyboardSelect::keyRawPressed);
            connect(this->map_of_raw_keyboards[name], &InputKeyboardRawController::signalRawKeyReleased, this, &InputKeyboardSelect::keyRawReleased);
        }
    }
}

void InputKeyboardSelect::detectDisplayInput()
{
    if (qgetenv("XDG_SESSION_TYPE") == "x11")
    {
        this->display_input_type = DisplayInput::X11;
    }
    /*
    else if (qgetenv("XDG_SESSION_TYPE") == "wayland" && qgetenv("WAYLAND_DISPLAY").startsWith("wayland"))
    {
        this->display_input_type = DisplayInput::Wayland;
    }
    */
    else
    {
        this->display_input_type = DisplayInput::Unsupported;
    }
}

void InputKeyboardSelect::keyboardRescan()
{
    detectDisplayInput();
    
    if (this->button_keyboard_lock->isDown())
        this->button_keyboard_lock->setDown(false);
    
    this->combo_keyboard_input_labels[KeyboardSelection::None] = "[Disabled]";
    this->combo_keyboard_input_labels[KeyboardSelection::Native] = "Qt native";
    this->combo_keyboard_input_labels[KeyboardSelection::Default] = "Qt default";
    this->combo_keyboard_input_labels[KeyboardSelection::Detect] = "[Detect RAW event]";
    
    this->keyboard_raw = new InputKeyboardRawMeta;
    QList<QString> keyboards = keyboard_raw->getKeyboardNames();
    
    instantiateRawKeyboards(keyboards);
    
    for (int i=this->combo_keyboard_input_labels.size()-1; i >= 0; i--)
    {
        keyboards.prepend(this->combo_keyboard_input_labels[i]);
    }
    this->combo_keyboard_selector->blockSignals(true);
    this->combo_keyboard_selector->clear();
    this->combo_keyboard_selector->addItems(keyboards);
    this->combo_keyboard_selector->blockSignals(false);
    
    if (this->display_input_type != DisplayInput::Unsupported)
    {
        QVariant v(0);
        //this->combo_keyboard_selector->setItemData(KeyboardSelection::Native, v, Qt::UserRole - 1);
        //this->combo_keyboard_selector->model()->setData(KeyboardSelection::Native, v, Qt::UserRole - 1);
    }
}

bool InputKeyboardSelect::isKeyboardLocked()
{
    return this->keyboard_locked;
}

void InputKeyboardSelect::instantiateRawKeyboards(QList<QString> keyboard_names)
{
    cleanupRawKeyboards();
    
    for (int i=0; i < keyboard_names.length(); i++)
    {
        this->map_of_raw_keyboards[keyboard_names.at(i)] = new InputKeyboardRawController;
    }
}
void InputKeyboardSelect::cleanupRawKeyboards()
{
    QList<QString> keys = this->map_of_raw_keyboards.keys();
    for (int i=0; i < keys.length(); i++)
    {
        this->map_of_raw_keyboards[keys.at(i)]->deleteLater();
    }
    
    this->map_of_raw_keyboards.clear();
}

void InputKeyboardSelect::keyRawPressed(int keycode)
{
    emit keyRawPressedSignal(keycode);
}
void InputKeyboardSelect::keyRawReleased(int keycode)
{
    emit keyRawReleasedSignal(keycode);
}
