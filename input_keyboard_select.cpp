#include "input_keyboard_select.h"

InputKeyboardSelect::InputKeyboardSelect(ComboKeyboardSelect *combo_keyboard_selector, QPushButton *button_keyboard_lock, QPushButton *button_keyboard_rescan, QObject *parent)
    : QObject{parent}
{
    this->combo_keyboard_selector = combo_keyboard_selector;
    this->button_keyboard_lock = button_keyboard_lock;
    
    connect(this->combo_keyboard_selector, &QComboBox::currentIndexChanged, this, &InputKeyboardSelect::keyboardSelectionChanged);
    
    connect(button_keyboard_rescan, &QPushButton::clicked, this, &InputKeyboardSelect::keyboardRescan);
    
    //connect(this->keyboard_raw, &InputKeyboardRaw::deviceNotAvailable, this, &MainTabs::deviceNotAvailable);
    //connect(this->keyboard_raw, &InputKeyboardRaw::rawKeyPressedSignal, this, &MainTabs::rawKeyPressed);
    //connect(this->keyboard_raw, &InputKeyboardRaw::rawKeyReleasedSignal, this, &MainTabs::rawKeyReleased);
}

void InputKeyboardSelect::keyboardSelectionChanged(int index)
{
    if (index == KeyboardSelection::None)
    {
        qDebug() << "NONE";
        this->button_keyboard_lock->setDisabled(true);
        emit keyboardSelectionChangedSignal(index);
    }
    else if (index == KeyboardSelection::Native)
    {
        qDebug() << "Native";
        this->button_keyboard_lock->setDisabled(false);
        emit keyboardSelectionChangedSignal(index);
    }
    else if (index == KeyboardSelection::Default)
    {
        qDebug() << "Default";
        this->button_keyboard_lock->setDisabled(false);
        emit keyboardSelectionChangedSignal(index);
    }
    else if (index == KeyboardSelection::Detect)
    {
        qDebug() << "Detect";
        this->button_keyboard_lock->setDisabled(true);
        emit keyboardSelectionChangedSignal(index);
    }
    // some RAW-Keyboard selected
    else
    {
        qDebug() << "RAW";
        this->button_keyboard_lock->setDisabled(false);
        emit keyboardSelectionChangedSignal(index);
        
        
    }
}

void InputKeyboardSelect::keyboardRescan()
{
    /*
    if (this->keyboard_locked)
        toggleKeyboardLock();
        */
    if (this->button_keyboard_lock->isDown())
        this->button_keyboard_lock->setDown(false);
    
    this->combo_keyboard_input_labels[KeyboardSelection::None] = "[Disabled]";
    this->combo_keyboard_input_labels[KeyboardSelection::Native] = "Qt native";
    this->combo_keyboard_input_labels[KeyboardSelection::Default] = "Qt default";
    this->combo_keyboard_input_labels[KeyboardSelection::Detect] = "[Detect RAW event]";
    
    InputKeyboardRaw *keyboard_raw = new InputKeyboardRaw;
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
        this->map_of_raw_keyboards[keyboard_names.at(i)] = new InputKeyboardRaw;
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
