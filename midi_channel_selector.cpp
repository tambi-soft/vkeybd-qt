#include "midi_channel_selector.h"

MIDIChannelSelector::MIDIChannelSelector(QList<InterfaceAudio *> list_of_audio_interfaces, QWidget *parent) : QWidget(parent)
{
    //this->audio = audio;
    //this->list_of_midi_outputs.append(list_of_audio_interfaces);
    this->list_of_midi_outputs = list_of_audio_interfaces;
    
    drawGUI();
    
    installEventFilter(this);
    
    setObjectName("midi_channel_selector");
    //setAttribute(Qt::WA_TranslucentBackground);
    QFile css_file(":css_allen_heath");
    if (css_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        setStyleSheet(css_file.readAll());
    }
}

void MIDIChannelSelector::setListOfAudioOutputs(QList<InterfaceAudio *> list_of_audio_interfaces)
{
    this->list_of_midi_outputs = list_of_audio_interfaces;
}

void MIDIChannelSelector::drawGUI()
{
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    grid->setSizeConstraint( QLayout::SetFixedSize );
    
    QLabel *label_channel = new QLabel("Channel");
    QLabel *label_output = new QLabel("Output");
    QLabel *label_volume = new QLabel("Volume");
    QLabel *label_pan = new QLabel("Pan");
    QLabel *label_key_shift = new QLabel("Key Shift");
    QLabel *label_key_min = new QLabel("Key Min");
    QLabel *label_key_max = new QLabel("Key Max");
    QLabel *label_instrument_group = new QLabel("Instrument Group");
    QLabel *label_instrument = new QLabel("Instrument");
    QLabel *label_midi_group = new QLabel("MSB");
    QLabel *label_midi_bank = new QLabel("LSB");
    QLabel *label_portamento_time = new QLabel("Port. Time");
    label_portamento_time->setToolTip("Portamento Time");
    QLabel *label_attack = new QLabel("Attack");
    QLabel *label_release = new QLabel("Release");
    QLabel *label_tremolo = new QLabel("Tremolo");
    
    grid->addWidget(label_channel, 0, 0);
    grid->addWidget(label_output, 0, 1);
    grid->addWidget(label_volume, 0, 2);
    grid->addWidget(label_pan, 0, 3);
    grid->addWidget(label_key_shift, 0, 4);
    grid->addWidget(label_key_min, 0, 5);
    grid->addWidget(label_key_max, 0, 6);
    grid->addWidget(label_instrument_group, 0, 7);
    grid->addWidget(label_instrument, 0, 8);
    grid->addWidget(label_midi_group, 0, 9);
    grid->addWidget(label_midi_bank, 0, 10);
    grid->addWidget(label_portamento_time, 0, 11);
    grid->addWidget(label_attack, 0, 12);
    grid->addWidget(label_release, 0, 13);
    grid->addWidget(label_tremolo, 0, 14);
    
    for (int i=1; i <= 16; i++)
    {
        QCheckBox *check = new QCheckBox(QString::number(i));
        
        QComboBox *combo_midi_output = new QComboBox;
        connect(combo_midi_output, &QComboBox::currentTextChanged, this, &MIDIChannelSelector::addNewAudioInterface);
        combo_midi_output->setObjectName("midi_output_selector");
        
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
        slider_pan->setObjectName("slider_pan");
        
        connect(slider_pan, &QSlider::sliderPressed, this, [slider_pan]{ slider_pan->setValue(64); });
        this->list_of_pan_sliders.append(slider_pan);
        
        MIDIKeyShiftWidget *key_shift = new MIDIKeyShiftWidget;
        QSpinBox *key_min = new QSpinBox();
        QSpinBox *key_max = new QSpinBox();
        //QSpinBox *key_shift = new QSpinBox();
        
        key_min->setMaximum(127);
        key_max->setMaximum(127);
        
        key_min->setValue(0);
        key_max->setValue(127);
        
        QComboBox *combo_instrument_group = new QComboBox;
        QList<QString> list_instrument_group = this->midi_sounds_list->getInstrumentGroups();
        combo_instrument_group->addItems(list_instrument_group);
        combo_instrument_group->setPlaceholderText("[non-default MSB/LSB set]");
        combo_instrument_group->setObjectName("instrument_selector");
        
        QComboBox *combo_instrument = new QComboBox;
        QList<QString> list_instrument = this->midi_sounds_list->getInstrumentsForGroupMIDIv1(list_instrument_group.at(0));
        combo_instrument->addItems(list_instrument);
        combo_instrument->setPlaceholderText("[non-default MSB/LSB set]");
        combo_instrument->setObjectName("instrument_selector");
        
        connect(combo_instrument_group, &QComboBox::currentTextChanged, this, [this, i, combo_instrument_group, combo_instrument]{ MIDIChannelSelector::instrumentGroupChanged(i-1, combo_instrument_group, combo_instrument); });
        //connect(combo_instrument_group, &QComboBox::currentTextChanged, this, [this, i, combo_instrument]{ MIDIChannelSelector::instrumentChanged(i-1, combo_instrument->currentText()); });
        
        connect(combo_instrument, &QComboBox::currentTextChanged, this, [this, i, combo_instrument]{ MIDIChannelSelector::instrumentChanged(i-1, combo_instrument->currentText()); });
        
        QSpinBox *midi_instrument_msb = new QSpinBox;
        QSpinBox *midi_instrument_lsb = new QSpinBox;
        midi_instrument_msb->setRange(0, 127);
        midi_instrument_lsb->setRange(0, 127);
        midi_instrument_msb->setObjectName("instrument_selector");
        midi_instrument_lsb->setObjectName("instrument_selector");
        connect(
                    midi_instrument_msb,
                    &QSpinBox::textChanged,
                    this,
                    [this, i, midi_instrument_msb, midi_instrument_lsb]{
                        MIDIChannelSelector::instrumentChangedNumeric(
                                    i-1,
                                    midi_instrument_msb->value(),
                                    midi_instrument_lsb->value()
                        );
                    }
        );
        connect(
                    midi_instrument_lsb,
                    &QSpinBox::textChanged,
                    this,
                    [this, i, midi_instrument_msb, midi_instrument_lsb]{
                        MIDIChannelSelector::instrumentChangedNumeric(
                                    i-1,
                                    midi_instrument_msb->value(),
                                    midi_instrument_lsb->value()
                        );
                    }
        );
        
        //QDial *dial_portamento = new QDial();
        //dial_portamento->resize(20, 20);
        QSlider *slider_portamento = new QSlider;
        slider_portamento->setOrientation(Qt::Horizontal);
        slider_portamento->setRange(0, 127);
        connect(slider_portamento, &QSlider::valueChanged, this, [this, i, slider_portamento]{ MIDIChannelSelector::portamentoChanged(i-1, slider_portamento->value()); });
        this->list_of_portamentos.append(slider_portamento);
        
        QSlider *slider_attack = new QSlider;
        slider_attack->setOrientation(Qt::Horizontal);
        slider_attack->setRange(0, 127);
        connect(slider_attack, &QSlider::valueChanged, this, [this, i, slider_attack]{ MIDIChannelSelector::attackChanged(i-1, slider_attack->value()); });
        this->list_of_attacks.append(slider_attack);
        
        QSlider *slider_release = new QSlider;
        slider_release->setOrientation(Qt::Horizontal);
        slider_release->setRange(0, 127);
        connect(slider_release, &QSlider::valueChanged, this, [this, i, slider_release]{ MIDIChannelSelector::releaseChanged(i-1, slider_release->value()); });
        this->list_of_releases.append(slider_release);
        
        QSlider *slider_tremolo = new QSlider;
        slider_tremolo->setOrientation(Qt::Horizontal);
        slider_tremolo->setRange(0, 127);
        connect(slider_tremolo, &QSlider::valueChanged, this, [this, i, slider_tremolo]{ MIDIChannelSelector::tremoloChanged(i-1, slider_tremolo->value()); });
        this->list_of_tremolos.append(slider_tremolo);
        
        grid->addWidget(check, i, 0);
        grid->addWidget(combo_midi_output, i, 1);
        grid->addWidget(slider_volume, i, 2);
        grid->addWidget(slider_pan, i, 3);
        grid->addWidget(key_shift, i, 4);
        
        grid->addWidget(key_min, i, 5);
        grid->addWidget(key_max, i, 6);
        
        grid->addWidget(combo_instrument_group, i, 7);
        grid->addWidget(combo_instrument, i, 8);
        grid->addWidget(midi_instrument_msb, i, 9);
        grid->addWidget(midi_instrument_lsb, i, 10);
        
        grid->addWidget(slider_portamento, i, 11);
        
        grid->addWidget(slider_attack, i, 12);
        grid->addWidget(slider_release, i, 13);
        
        grid->addWidget(slider_tremolo, i, 14);
        
        if (i==1)
        {
            check->setChecked(true);
        }
        
        this->list_of_checkboxes.append(check);
        this->list_of_midi_output_combos.append(combo_midi_output);
        this->list_of_keyshifts.append(key_shift);
        this->list_of_key_mins.append(key_min);
        this->list_of_key_maxs.append(key_max);
        this->list_of_instrument_groups.append(combo_instrument_group);
        this->list_of_instrument_banks.append(combo_instrument);
        this->list_of_msb.append(midi_instrument_msb);
        this->list_of_lsb.append(midi_instrument_lsb);
    }
    
    populateAudioCombos();
    
    QPushButton *button_test_note = new QPushButton("Play Test Note");
    connect(button_test_note, &QPushButton::pressed, this, &MIDIChannelSelector::playTestNote);
    connect(button_test_note, &QPushButton::released, this, &MIDIChannelSelector::stopTestNote);
    grid->addWidget(button_test_note, 17, 0, 1, 14);
}

QList<QMap<QString,QVariant>> MIDIChannelSelector::listOfChannels(bool only_activated)
{
    QList<QMap<QString,QVariant>> result;
    
    for (int i=0; i < this->list_of_checkboxes.length(); i++)
    {
        QCheckBox *check = this->list_of_checkboxes.at(i);
        if (!only_activated || check->isChecked())
        {
            QMap<QString,QVariant> map;
            
            map["channel"] = i;
            
            map["interface_index"] = this->list_of_midi_output_combos.at(i)->currentIndex();
            
            map["activated"] = this->list_of_checkboxes.at(i)->isChecked();
            
            int volume = this->list_of_volume_sliders.at(i)->value();
            map["volume"] = volume;
            
            int pan = this->list_of_pan_sliders.at(i)->value();
            map["pan"] = pan;
            
            int key_shift = this->list_of_keyshifts.at(i)->value();
            map["key_shift"] = key_shift;
            
            int key_min = this->list_of_key_mins.at(i)->value();
            map["key_min"] = key_min;
            
            int key_max = this->list_of_key_maxs.at(i)->value();
            map["key_max"] = key_max;
            
            int instrument_msb = this->list_of_msb.at(i)->value();
            map["instrument_msb"] = instrument_msb;
            
            int instrument_lsb = this->list_of_lsb.at(i)->value();
            map["instrument_lsb"] = instrument_lsb;
            
            QString instrument_name = this->list_of_instrument_banks.at(i)->currentText();
            map["instrument_name"] = instrument_name;
            
            int portamento_time = this->list_of_portamentos.at(i)->value();
            map["portamento_time"] = portamento_time;
            
            int attack = this->list_of_attacks.at(i)->value();
            map["attack"] = attack;
            
            int release = this->list_of_releases.at(i)->value();
            map["release"] = release;
            
            int tremolo = this->list_of_tremolos.at(i)->value();
            map["tremolo"] = tremolo;
            
            result.append(map);
        }
    }
    
    return result;
}

void MIDIChannelSelector::restoreParams(QMap<QString,QVariant> data)
{
    for (int i=0; i < this->list_of_checkboxes.length(); i++)
    {
        QMap<QString,QVariant> channel = data[QString::number(i)].toMap();
        
        this->list_of_checkboxes.at(i)->setChecked(channel["activated"].toBool());
        
        this->list_of_volume_sliders.at(i)->setValue(channel["volume"].toInt());
        this->list_of_pan_sliders.at(i)->setValue(channel["pan"].toInt());
        this->list_of_keyshifts.at(i)->setValue(channel["key_shift"].toInt());
        this->list_of_key_mins.at(i)->setValue(channel["key_min"].toInt());
        this->list_of_key_maxs.at(i)->setValue(channel["key_max"].toInt());
        this->list_of_msb.at(i)->setValue(channel["instrument_msb"].toInt());
        this->list_of_lsb.at(i)->setValue(channel["instrument_lsb"].toInt());
        
        this->list_of_portamentos.at(i)->setValue(channel["portamento_time"].toInt());
        this->list_of_attacks.at(i)->setValue(channel["attack"].toInt());
        this->list_of_releases.at(i)->setValue(channel["release"].toInt());
        this->list_of_tremolos.at(i)->setValue(channel["tremolo"].toInt());
        
        int audio_interface_index = channel["interface_index"].toInt();
        if (audio_interface_index >= 1)
        {
            addNewAudioInterface(ADD_NEW_AUDIO_OUTPUT_LABEL);
            this->list_of_midi_output_combos.at(i)->setCurrentIndex(audio_interface_index);
        }
    }
}

void MIDIChannelSelector::populateAudioCombos()
{
    QList<QString> list_of_audio_output_labels;
    for (int i=0; i < this->list_of_midi_outputs.length(); i++)
    {
        list_of_audio_output_labels.append(this->list_of_midi_outputs.at(i)->label());
    }
    list_of_audio_output_labels.append(ADD_NEW_AUDIO_OUTPUT_LABEL);
    
    for (int i=0; i < this->list_of_midi_output_combos.length(); i++)
    {
        QComboBox *combo = this->list_of_midi_output_combos.at(i);
        combo->blockSignals(true);
        int current_index = 0;
        if (combo->count() > 0)
        {
            current_index = combo->currentIndex();
        }
        combo->clear();
        combo->addItems(list_of_audio_output_labels);
        if (combo->count() > 0)
        {
            combo->setCurrentIndex(current_index);
        }
        combo->blockSignals(false);
    }
}

void MIDIChannelSelector::addNewAudioInterface(QString text)
{
    if (text == ADD_NEW_AUDIO_OUTPUT_LABEL)
    {
        QString label = this->list_of_midi_outputs.at(0)->label();
        QString length = QString::number(this->list_of_midi_outputs.length());
        qDebug() << label;
        //emit newAudioInterfaceRequested(label+"-"+QString::number(id));
        emit newAudioInterfaceRequested(label+"-"+length);
        
        qDebug() << text << "aaaaaaaaaaaaaaaaaaaaaa";
        populateAudioCombos();
    }
}

InterfaceAudio* MIDIChannelSelector::selectedAudioInterface(int channel)
{
    QComboBox *combo = this->list_of_midi_output_combos.at(channel);
    return this->list_of_midi_outputs.at(combo->currentIndex());
}

void MIDIChannelSelector::volumeSliderMoved(int channel, int volume)
{
    volume = volume * this->volume_dca / 100;
    if (volume > 127)
    {
        volume = 127;
    }
    
    InterfaceAudio *audio = selectedAudioInterface(channel);
    audio->setVolumeChangeEvent(channel, volume);
}
void MIDIChannelSelector::volumeDCAChanged(int value)
{
    qDebug() << "ararararar";
    this->volume_dca = value;
    
    QList<QMap<QString,QVariant>> channels = listOfChannels(false);
    for (int i=0; i < channels.length(); i++)
    {
        int channel = channels.at(i)["channel"].toInt();
        int volume = channels.at(i)["volume"].toInt();
        
        volumeSliderMoved(channel, volume);
    }
}

void MIDIChannelSelector::panSliderMoved(int channel, int value)
{
    InterfaceAudio *audio = selectedAudioInterface(channel);
    audio->setPanChangeEvent(channel, value);
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
    
    //instrumentChanged(channel, combo_instrument->currentText());
}

void MIDIChannelSelector::instrumentChanged(int channel, QString instrument)
{
    qDebug() << "changed-: "+instrument;
    
    QMap<QString,int> codes = this->midi_sounds_list->getMIDICodesForInstrument(instrument);
    int instrument_msb = codes["msb"] - 1;
    int instrument_lsb = codes["lsb"];
    
    this->list_of_msb.at(channel)->setValue(instrument_msb);
    this->list_of_lsb.at(channel)->setValue(instrument_lsb);
}

void MIDIChannelSelector::instrumentChangedNumeric(int channel, int instrument_msb, int instrument_lsb)
{
    InterfaceAudio *audio = selectedAudioInterface(channel);
    audio->setProgramChangeEvent(channel, instrument_msb, instrument_lsb);
    
    qDebug() << "changed+: "+QString::number(channel)+" "+QString::number(instrument_msb)+" "+QString::number(instrument_lsb);
    audio->setProgramChangeEvent(channel, instrument_msb, instrument_lsb);
    
    // change spin boxes for group and bank accordingly
    QMap<QString,QString> names = this->midi_sounds_list->getInstrumentForMIDICodes(instrument_msb, instrument_lsb);
    qDebug() << names;
    
    this->list_of_instrument_banks.at(channel)->blockSignals(true);
    if (names["group"] == "")
    {
        this->list_of_instrument_groups.at(channel)->setCurrentIndex(-1);
    }
    else
    {
        this->list_of_instrument_groups.at(channel)->setCurrentText(names["group"]);
    }
    
    if (names["instrument"] == "")
    {
        this->list_of_instrument_banks.at(channel)->setCurrentIndex(-1);
    }
    else
    {
        this->list_of_instrument_banks.at(channel)->setCurrentText(names["instrument"]);
    }
    this->list_of_instrument_banks.at(channel)->blockSignals(false);
}

void MIDIChannelSelector::portamentoChanged(int channel, int value)
{
    InterfaceAudio *audio = selectedAudioInterface(channel);
    audio->setPortamentoChanged(channel, value);
}

void MIDIChannelSelector::attackChanged(int channel, int value)
{
    InterfaceAudio *audio = selectedAudioInterface(channel);
    audio->setAttackChanged(channel, value);
}

void MIDIChannelSelector::releaseChanged(int channel, int value)
{
    InterfaceAudio *audio = selectedAudioInterface(channel);
    audio->setReleaseChanged(channel, value);
}

void MIDIChannelSelector::tremoloChanged(int channel, int value)
{
    InterfaceAudio *audio = selectedAudioInterface(channel);
    audio->setTremoloChanged(channel, value);
}

void MIDIChannelSelector::resendMIDIControls()
{
    QList<QMap<QString,QVariant>> channels = listOfChannels(true);
    for (int i=0; i < channels.length(); i++)
    {
        int channel = channels.at(i)["channel"].toInt();
        
        volumeSliderMoved(channel, channels.at(i)["volume"].toInt());
        panSliderMoved(channel, channels.at(i)["pan"].toInt());
        InterfaceAudio *audio = selectedAudioInterface(channel);
        audio->setProgramChangeEvent(
                    channel,
                    channels.at(i)["instrument_msb"].toInt(),
                    channels.at(i)["instrument_lsb"].toInt()
                    );
        portamentoChanged(channel, channels.at(i)["portamento_time"].toInt());
        attackChanged(channel, channels.at(i)["attack"].toInt());
        releaseChanged(channel, channels.at(i)["release"].toInt());
    }
}

bool MIDIChannelSelector::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::KeyPress)//|| ev->type() == QEvent::KeyRelease)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(ev);
        
        if (event->isAutoRepeat() == false)
        {
            if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
            {
                setFocus();
                
                return false;
            }
            else if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
            {
                int channel_number = event->key() - Qt::Key_0;
                
                if (event->modifiers() & Qt::AltModifier)
                {
                    channel_number += 10;
                }
                else if (event->key() == Qt::Key_0)
                {
                    channel_number = 10;
                }
                
                if (channel_number <= this->list_of_checkboxes.length())
                {
                    this->list_of_checkboxes.at(channel_number-1)->toggle();
                }
                
                return true;
            }
            else if (event->key() == Qt::Key_Escape)
            {
                this->hide();
                emit closed();
                
                return true;
            }
            
            emit eventFiltered(obj, ev);
            return true;
        }
    }
    else if (ev->type() == QEvent::KeyRelease)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(ev);
        
        if (event->isAutoRepeat() == false)
        {
            emit eventFiltered(obj, ev);
            return true;
        }
    }
    
    return false;
}

void MIDIChannelSelector::playTestNote()
{
    QList<QMap<QString,QVariant>> channels = listOfChannels(true);
    for (int i=0; i < channels.length(); i++)
    {
        InterfaceAudio *audio = selectedAudioInterface(i);
        audio->keyPressEvent(i, 60);
    }
}
void MIDIChannelSelector::stopTestNote()
{
    QList<QMap<QString,QVariant>> channels = listOfChannels(true);
    for (int i=0; i < channels.length(); i++)
    {
        InterfaceAudio *audio = selectedAudioInterface(i);
        audio->keyReleaseEvent(i, 60);
    }
}
