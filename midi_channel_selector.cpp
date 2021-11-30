#include "midi_channel_selector.h"

MIDIChannelSelector::MIDIChannelSelector(InterfaceAudio *audio, QWidget *parent) : QWidget(parent)
{
    this->audio = audio;
    
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    
    QLabel *channel_label = new QLabel("Channel");
    QLabel *label_volume = new QLabel("Volume");
    QLabel *label_pan = new QLabel("Pan");
    QLabel *label_key_shift = new QLabel("Key Shift");
    QLabel *key_min_label = new QLabel("Key Min");
    QLabel *key_max_label = new QLabel("Key Max");
    QLabel *label_instrument_group = new QLabel("Instrument Group");
    QLabel *label_instrument = new QLabel("Instrument");
    QLabel *label_midi_group = new QLabel("MSB");
    QLabel *label_midi_bank = new QLabel("LSB");
    QLabel *portamento_time = new QLabel("Port. Time");
    portamento_time->setToolTip("Portamento Time");
    QLabel *label_attack = new QLabel("Attack");
    QLabel *label_release = new QLabel("Release");
    
    
    grid->addWidget(channel_label, 0, 0);
    grid->addWidget(label_volume, 0, 1);
    grid->addWidget(label_pan, 0, 2);
    grid->addWidget(label_key_shift, 0, 3);
    grid->addWidget(key_min_label, 0, 4);
    grid->addWidget(key_max_label, 0, 5);
    grid->addWidget(label_instrument_group, 0, 6);
    grid->addWidget(label_instrument, 0, 7);
    grid->addWidget(label_midi_group, 0, 8);
    grid->addWidget(label_midi_bank, 0, 9);
    grid->addWidget(portamento_time, 0, 10);
    grid->addWidget(label_attack, 0, 11);
    grid->addWidget(label_release, 0, 12);
    
    for (int i=1; i<=16; i++)
    {
        QCheckBox *check = new QCheckBox(QString::number(i));
        
        QSlider *slider_volume = new QSlider;
        slider_volume->setOrientation(Qt::Horizontal);
        slider_volume->setMinimum(0);
        slider_volume->setMaximum(127);
        slider_volume->setValue(127);
        connect(slider_volume, &QSlider::valueChanged, this, [this, i, slider_volume]{ MIDIChannelSelector::volumeSliderMoved(i-1, slider_volume->value()); });
        this->list_of_volume_sliders.append(slider_volume);
        
        QSlider *slider_pan = new QSlider;
        slider_pan->setOrientation(Qt::Horizontal);
        slider_pan->setRange(0, 127);
        slider_pan->setValue(64);
        connect(slider_pan, &QSlider::valueChanged, this, [this, i, slider_pan]{ MIDIChannelSelector::panSliderMoved(i-1, slider_pan->value()); });
        //slider_pan->setStyleSheet("QSlider::groove:horizontal {background-color:red;}");
        QString style_pan = "QSlider::groove:horizontal {"
                "border: 1px solid #999999;"
                "height: 20px;"
                "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                "margin: 2px 0;"
                "}"
                "QSlider::handle:horizontal {"
                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                "border: 1px solid #5c5c5c;"
                "width: 15px;"
                "margin: -2px 0px;"
                "}";
        slider_pan->setStyleSheet(style_pan);
        connect(slider_pan, &QSlider::sliderPressed, this, [slider_pan]{ slider_pan->setValue(64); });
        
        MIDIKeyShiftWidget *key_shift = new MIDIKeyShiftWidget;
        QSpinBox *key_min = new QSpinBox();
        QSpinBox *key_max = new QSpinBox();
        //QSpinBox *key_shift = new QSpinBox();
        
        key_min->setMaximum(127);
        key_max->setMaximum(127);
        
        key_min->setValue(0);
        key_max->setValue(127);
        
        QComboBox *combo_instrument_group = new QComboBox;
        //QStringList list_instrument_group = MIDISoundsList::getCategories();
        QList<QString> list_instrument_group = this->midi_sounds_list->getInstrumentGroups();
        combo_instrument_group->addItems(list_instrument_group);
        
        QComboBox *combo_instrument = new QComboBox;
        QList<QString> list_instrument = this->midi_sounds_list->getInstrumentsForGroupMIDIv1(list_instrument_group.at(0));
        combo_instrument->addItems(list_instrument);
        
        connect(combo_instrument_group, &QComboBox::currentTextChanged, this, [this, i, combo_instrument_group, combo_instrument]{ MIDIChannelSelector::instrumentGroupChanged(i-1, combo_instrument_group, combo_instrument); });
        connect(combo_instrument, &QComboBox::currentTextChanged, this, [this, i, combo_instrument]{ MIDIChannelSelector::instrumentChanged(i-1, combo_instrument); });
        
        QSpinBox *midi_group = new QSpinBox;
        QSpinBox *midi_bank = new QSpinBox;
        midi_group->setRange(0, 127);
        midi_bank->setRange(0, 127);
        connect(midi_group, &QSpinBox::textChanged, this, [this, i, midi_group, midi_bank]{ MIDIChannelSelector::instrumentChangedNumeric(i-1, midi_group->value(), midi_bank->value()); });
        
        //QDial *dial_portamento = new QDial();
        //dial_portamento->resize(20, 20);
        QSlider *slider_portamento = new QSlider;
        slider_portamento->setOrientation(Qt::Horizontal);
        slider_portamento->setRange(0, 127);
        connect(slider_portamento, &QSlider::valueChanged, this, [this, i, slider_portamento]{ MIDIChannelSelector::portamentoChanged(i-1, slider_portamento); });
        
        QSlider *slider_attack = new QSlider;
        slider_attack->setOrientation(Qt::Horizontal);
        slider_attack->setRange(0, 127);
        connect(slider_attack, &QSlider::valueChanged, this, [this, i, slider_attack]{ MIDIChannelSelector::attackChanged(i-1, slider_attack); });
        
        QSlider *slider_release = new QSlider;
        slider_release->setOrientation(Qt::Horizontal);
        slider_release->setRange(0, 127);
        connect(slider_release, &QSlider::valueChanged, this, [this, i, slider_release]{ MIDIChannelSelector::releaseChanged(i-1, slider_release); });
        
        grid->addWidget(check, i, 0);
        grid->addWidget(slider_volume, i, 1);
        grid->addWidget(slider_pan, i, 2);
        grid->addWidget(key_shift, i, 3);
        
        grid->addWidget(key_min, i, 4);
        grid->addWidget(key_max, i, 5);
        
        grid->addWidget(combo_instrument_group, i, 6);
        grid->addWidget(combo_instrument, i, 7);
        grid->addWidget(midi_group, i, 8);
        grid->addWidget(midi_bank, i, 9);
        
        grid->addWidget(slider_portamento, i, 10);
        
        grid->addWidget(slider_attack, i, 11);
        grid->addWidget(slider_release, i, 12);
        
        if (i==1)
        {
            check->setChecked(true);
        }
        
        QString stylesheet = "QCheckBox:indicator:checked {"
                             "  color: white;"
                             "  background-color: black;"
                             "}";
        
        check->setStyleSheet(stylesheet);
        
        this->list_of_checkboxes.append(check);
        this->list_of_keyshifts.append(key_shift);
        this->list_of_key_mins.append(key_min);
        this->list_of_key_maxs.append(key_max);
        this->list_of_instrument_groups.append(combo_instrument_group);
        this->list_of_instrument_banks.append(combo_instrument);
        this->list_of_msb.append(midi_group);
        this->list_of_lsb.append(midi_bank);
    }
    
    installEventFilter(this);
}

QList<QMap<QString,int>> MIDIChannelSelector::getListOfActivatedChannels()
{
    QList<QMap<QString,int>> result;
    
    for (int i=0; i < this->list_of_checkboxes.length(); i++)
    {
        QCheckBox *check = this->list_of_checkboxes.at(i);
        if (check->isChecked())
        {
            QMap<QString,int> map;
            
            map["channel"] = i;
            
            int key_shift = this->list_of_keyshifts.at(i)->spin_key->value();
            map["key_shift"] = key_shift;
            
            int key_min = this->list_of_key_mins.at(i)->value();
            map["key_min"] = key_min;
            
            int key_max = this->list_of_key_maxs.at(i)->value();
            map["key_max"] = key_max;
            
            int volume = this->list_of_volume_sliders.at(i)->value();
            map["volume"] = volume;
            
            result.append(map);
        }
    }
    
    return result;
}

void MIDIChannelSelector::volumeSliderMoved(int channel, int volume)
{
    volume = volume * this->volume_dca / 100;
    if (volume > 127)
    {
        volume = 127;
    }
    
    this->audio->setVolumeChangeEvent(channel, volume);
}
void MIDIChannelSelector::volumeDCAChanged(int value)
{
    this->volume_dca = value;
    
    QList<QMap<QString,int>> channels = getListOfActivatedChannels();
    for (int i=0; i < channels.length(); i++)
    {
        int channel = channels.at(i)["channel"];
        int volume = channels.at(i)["volume"];
        
        volumeSliderMoved(channel, volume);
    }
}

void MIDIChannelSelector::panSliderMoved(int channel, int value)
{
    this->audio->setPanChangeEvent(channel, value);
}

void MIDIChannelSelector::instrumentGroupChanged(int channel, QComboBox *combo_group, QComboBox *combo_instrument)
{
    combo_instrument->blockSignals(true);
    combo_instrument->clear();
    
    QString group = combo_group->currentText();
    //QList<QString> instruments = this->midi_sounds_list->getInstrumentsForGroupMIDIv1(group);
    QList<QString> instruments = this->midi_sounds_list->getInstrumentsForGroupMIDIv2(group);
    
    combo_instrument->addItems(instruments);
    combo_instrument->blockSignals(false);
    
    instrumentChanged(channel, combo_instrument);
}

void MIDIChannelSelector::instrumentChanged(int channel, QComboBox *combo_instrument)
{
    QString name = combo_instrument->currentText();
    //if (name != (new MIDISoundsList())->BANK_NO_NAME)
    //{
        qDebug() << "changed: "+name;
        
        QList<int> codes = this->midi_sounds_list->getMIDICodesForInstrument(name);
        int program = codes.at(0) - 1;
        int bank = codes.at(1);
        
        this->audio->setProgramChangeEvent(channel, program, bank);
        
        // change msb and lsb spinbox accordingly but not send signals
        this->list_of_msb.at(channel)->blockSignals(true);
        this->list_of_msb.at(channel)->setValue(program);
        this->list_of_msb.at(channel)->blockSignals(false);
        
        this->list_of_lsb.at(channel)->blockSignals(true);
        this->list_of_lsb.at(channel)->setValue(bank);
        this->list_of_lsb.at(channel)->blockSignals(false);
    //}
}

void MIDIChannelSelector::instrumentChangedNumeric(int channel, int program, int bank)
{
    qDebug() << "changed: "+QString::number(channel)+" "+QString::number(bank);
    this->audio->setProgramChangeEvent(channel, program, bank);
    
    // change spin boxes for group and bank accordingly
    this->list_of_instrument_groups.at(channel)->blockSignals(true);
    this->list_of_instrument_groups.at(channel)->setCurrentIndex(program);
    this->list_of_instrument_groups.at(channel)->blockSignals(false);
    
    this->list_of_instrument_banks.at(channel)->blockSignals(true);
    this->list_of_instrument_banks.at(channel)->setCurrentIndex(bank);
    this->list_of_instrument_banks.at(channel)->blockSignals(true);
}

void MIDIChannelSelector::portamentoChanged(int channel, QSlider *slider)
{
    this->audio->setPortamentoChanged(channel, slider->value());
}

void MIDIChannelSelector::attackChanged(int channel, QSlider *slider)
{
    this->audio->setAttackChanged(channel, slider->value());
}

void MIDIChannelSelector::releaseChanged(int channel, QSlider *slider)
{
    this->audio->setReleaseChanged(channel, slider->value());
}

void MIDIChannelSelector::resendMIDIControls()
{
    QList<QMap<QString,int>> channels = getListOfActivatedChannels();
    for (int i=0; i < channels.length(); i++)
    {
        int channel = channels.at(i)["channel"];
        
        volumeSliderMoved(channel, channels.at(i)["volume"]);
        
    }
}

bool MIDIChannelSelector::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyRelease)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(ev);
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {
            setFocus();
            
            return false;
        }
        else
        {
            emit eventFiltered(obj, ev);
            
            return true;
        }
    }
    
    return false;
}





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
    
    connect(button_higher, &QPushButton::clicked, this, &MIDIKeyShiftWidget::higherShiftKeyPressed);
    connect(button_lower, &QPushButton::clicked, this, &MIDIKeyShiftWidget::lowerShiftKeyPressed);
    
    this->spin_key = new QSpinBox;
    this->spin_key->setMinimum(-64);
    this->spin_key->setMaximum(64);
    
    layout->addWidget(button_lower);
    layout->addWidget(this->spin_key);
    layout->addWidget(button_higher);
}

void MIDIKeyShiftWidget::lowerShiftKeyPressed()
{
    int val = this->spin_key->value();
    if (val >= -64+12)
    {
        val -= 12;
        this->spin_key->setValue(val);
    }
}

void MIDIKeyShiftWidget::higherShiftKeyPressed()
{
    int val = this->spin_key->value();
    if (val <= 64-12)
    {
        val += 12;
        this->spin_key->setValue(val);
    }
}
