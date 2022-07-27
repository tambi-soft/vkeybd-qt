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
    connect(this->spin_key, &QSpinBox::valueChanged, this, [this]{ emit valueChanged(this->spin_key->value()); });
    
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
    
    emit valueChanged(val);
}

void MIDIKeyShiftWidget::higherShiftKeyPressed()
{
    int val = this->spin_key->value();
    if (val <= 64-12)
    {
        val += 12;
        this->spin_key->setValue(val);
    }
    
    emit valueChanged(val);
}

int MIDIKeyShiftWidget::value()
{
    return this->spin_key->value();
}
void MIDIKeyShiftWidget::setValue(int value)
{
    qDebug() << "set value: " << value;
    this->spin_key->setValue(value);
    
    emit valueChanged(value);
}
