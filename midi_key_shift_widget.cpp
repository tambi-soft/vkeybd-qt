#include "midi_key_shift_widget.h"

MIDIKeyShiftWidget::MIDIKeyShiftWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    
    QPushButton *button_lower = new QPushButton("<<");
    QPushButton *button_higher = new QPushButton(">>");
    button_higher->setMaximumWidth(30);
    button_lower->setMaximumWidth(30);
    button_higher->setObjectName("key_shift_button");
    button_lower->setObjectName("key_shift_button");
    
    connect(button_higher, &QPushButton::clicked, this, &MIDIKeyShiftWidget::higherShiftKeyPressed);
    connect(button_lower, &QPushButton::clicked, this, &MIDIKeyShiftWidget::lowerShiftKeyPressed);
    
    this->spin_key = new QSpinBox;
    this->spin_key->setMinimum(-64);
    this->spin_key->setMaximum(64);
    this->spin_key->setObjectName("key_shift_spin");
    connect(this->spin_key, &QSpinBox::valueChanged, this, &MIDIKeyShiftWidget::valueChanged);
    
    layout->addWidget(button_lower);
    layout->addWidget(this->spin_key);
    layout->addWidget(button_higher);
    
    button_lower->setFocusPolicy(Qt::NoFocus);
    button_higher->setFocusPolicy(Qt::NoFocus);
}

void MIDIKeyShiftWidget::lowerShiftKeyPressed()
{
    int val = this->spin_key->value();
    if (val >= -64+12)
    {
        val -= 12;
        this->spin_key->setValue(val);
    }
    
    emit signalValueChanged(val);
}

void MIDIKeyShiftWidget::higherShiftKeyPressed()
{
    int val = this->spin_key->value();
    if (val <= 64-12)
    {
        val += 12;
        this->spin_key->setValue(val);
    }
    
    emit signalValueChanged(val);
}

int MIDIKeyShiftWidget::value()
{
    return this->spin_key->value();
}
void MIDIKeyShiftWidget::setValue(int value)
{
    this->spin_key->setValue(value);
    
    emit signalValueChanged(value);
}
void MIDIKeyShiftWidget::setValueFromGlobal(int value)
{
    // block signal to avoid sending setValueStart in valueChanged(int value)
    this->spin_key->blockSignals(true);
    this->spin_key->setValue(value);
    this->spin_key->blockSignals(false);
    
    emit signalValueChanged(value);
}
void MIDIKeyShiftWidget::valueChanged(int value)
{
    emit signalValueChanged(this->spin_key->value());
    
    setValueStart(value);
}

int MIDIKeyShiftWidget::valueStart()
{
    return this->value_start;
}
void MIDIKeyShiftWidget::setValueStart(int value)
{
    this->value_start = value;
}
