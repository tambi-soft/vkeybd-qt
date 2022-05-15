#include "orgelwerk.h"

Orgelwerk::Orgelwerk(int id, OutputSystem output, QString label, QWidget *parent) : QWidget(parent)
{
    this->id = id;
    this->audio_system = output;
    this->label = label;
    
    if (output == OutputSystem::Alsa)
    {
        this->list_of_audio_interfaces.append(new InterfaceAlsa("alsa-midi-"+QString::number(id+1)+"-"+label));
    }
    else if (output == OutputSystem::Jack)
    {
        this->list_of_audio_interfaces.append(new InterfaceJack("jack-midi-"+QString::number(id+1)+"-"+label));
    }
    else
    {
        this->list_of_audio_interfaces.append(new InterfaceAudio(""));
    }
    
    drawGUI();
    initInputThread();
    
    installEventFilter(this);
}

void Orgelwerk::drawGUI()
{
    //this->channels = new MIDIChannelSelector(this->interface_audio);
    this->channels = new MIDIChannelSelector(this->list_of_audio_interfaces);
    connect(this->channels, &MIDIChannelSelector::newAudioInterfaceRequested, this, &Orgelwerk::addNewAudioInterface);
    this->keys = new MIDIKeySelector;
    this->pitch = new MIDIPitchWheel;
    
    connect(this->channels, &MIDIChannelSelector::eventFiltered, this, &Orgelwerk::eventFilter);
    connect(this->channels, &MIDIChannelSelector::closed, this, &Orgelwerk::channelsSummaryUpdate);
    
    connect(this->pitch, &MIDIPitchWheel::pitchWheelMoved, this, &Orgelwerk::pitchWheelMoved);
    
    this->grid = new QGridLayout;
    setLayout(this->grid);
    
    this->group_keys = new QGroupBox("Keys");
    this->group_pitch = new QGroupBox("Pitch");
    
    QVBoxLayout *layout_keys = new QVBoxLayout;
    QHBoxLayout *layout_pitch = new QHBoxLayout;
    
    this->group_keys->setLayout(layout_keys);
    this->group_pitch->setLayout(layout_pitch);
    
    layout_keys->addWidget(this->keys);
    layout_pitch->addWidget(this->pitch);
    
    this->button_panic->setText("Panic! [Esc]");
    connect(this->button_panic, &QPushButton::clicked, this, &Orgelwerk::panicKeyPressed);
    this->button_panic->setObjectName("button_panic");
    
    this->button_stop_all->setText("Stop All [Del]");
    connect(this->button_stop_all, &QPushButton::clicked, this, &Orgelwerk::stopAllPressed);
    this->button_stop_all->setObjectName("button_stop");
    
    QLayout *layout_panic_stop = new QHBoxLayout;
    layout_panic_stop->addWidget(this->button_panic);
    layout_panic_stop->addWidget(this->button_stop_all);
    
    QLabel *label_key_shift_master = new QLabel("Key Shift");
    this->key_shift_master = new MIDIKeyShiftWidget;
    
    this->volume = new MIDIMasterVolume;
    connect(volume, &MIDIMasterVolume::sliderMoved, this, &Orgelwerk::volumeSliderMoved);
    
    //showChannelsReal(0);
    //showChannelsImage(1);
    showChannelsSummary(1);
    this->grid->addWidget(label_key_shift_master, 2, 0);
    this->grid->addWidget(this->key_shift_master, 3, 0);
    //this->grid->addWidget(key_shift_widget, 2, 0);
    this->grid->addWidget(volume, 2, 1, 2, 1);
    //this->grid->addWidget(label_volume_master, 2, 1);
    //this->grid->addWidget(this->slider_volume_master, 3, 1);
    this->grid->addWidget(this->group_keys, 4, 0, 1, 2);
    this->grid->addWidget(this->group_pitch, 5, 0, 1, 2);
    drawNotesKeyboard(6);
    drawPianoKeyboard(7);
    drawPCKeyboard(8);
    this->grid->addLayout(layout_panic_stop, 10, 0, 1, 2);
    
    //this->grid->setSizeConstraint( QLayout::SetFixedSize );
}

void Orgelwerk::drawNotesKeyboard(int grid_row)
{
    this->notes = new KeyboardNotes;
    
    this->grid->addWidget(this->notes, grid_row, 0, 1, 2);
}
void Orgelwerk::drawPianoKeyboard(int grid_row)
{
    this->piano = new KeyboardPiano;
    
    this->grid->addWidget(this->piano, grid_row, 0, 1, 2);
    this->piano->hide();
}
void Orgelwerk::drawPCKeyboard(int grid_row)
{
    this->pc = new KeyboardPC;
    connect(this->pc, &KeyboardPC::MIDIPress, this, &Orgelwerk::keyMIDIDown);
    connect(this->pc, &KeyboardPC::MIDIRelease, this, &Orgelwerk::keyMIDIUp);
    
    this->grid->addWidget(this->pc, grid_row, 0, 1, 2);
}

void Orgelwerk::showChannelsSummary(int grid_row)
{
    QGroupBox *group_channels = new QGroupBox("Channels");
    QGridLayout *layout_channels = new QGridLayout;
    group_channels->setLayout(layout_channels);
    
    this->midi_channels_summary = new MIDIChannelsSummary;
    
    this->button_channels_dialog->setText("Edit MIDI Channels");
    connect(button_channels_dialog, &QPushButton::clicked, this, &Orgelwerk::showChannelDetails);
    
    this->button_resend_midi->setText("Resend MIDI Settings [Ins]");
    connect(button_resend_midi, &QPushButton::clicked, this, &Orgelwerk::resendMIDIControls);
    
    //layout_channels->addWidget(scroll, 0, 0, 1, 2);
    layout_channels->addWidget(this->midi_channels_summary, 0, 0, 1, 2);
    layout_channels->addWidget(button_channels_dialog, 1, 0);
    layout_channels->addWidget(button_resend_midi, 1, 1);
    
    this->grid->addWidget(group_channels, grid_row, 0, 1, 2);
    
    channelsSummaryUpdate();
}
void Orgelwerk::channelsSummaryUpdate()
{
    QList<QMap<QString,QVariant>> data = this->channels->listOfChannels(true);
    
    this->midi_channels_summary->showData(data);
}

void Orgelwerk::initInputThread()
{
    //this->thread_input = new QThread(this);
    //this->worker_input = new InputKeyboardRawThread();
    //this->worker_input->moveToThread(this->thread_input);
    
    //this->thread_input->start();
    
    //this->worker_input->start();
}

Orgelwerk::~Orgelwerk()
{
    this->thread_input->exit();
}

void Orgelwerk::addNewAudioInterface(QString label)
{
    if (this->audio_system == OutputSystem::Alsa)
    {
        this->list_of_audio_interfaces.append(new InterfaceAlsa(label));
    }
    else if (this->audio_system == OutputSystem::Jack)
    {
        this->list_of_audio_interfaces.append(new InterfaceJack(label));
    }
    
    this->channels->setListOfAudioOutputs(this->list_of_audio_interfaces);
}
/*
void Orgelwerk::keyDown(int keycode)
{
    
    this->number_of_keys_down++;
    
    //qDebug() << "keyDown: "+this->label+" "+ QString::number(keycode);
    this->pc->keyDown(keycode);
}
void Orgelwerk::keyUp(int keycode)
{
    if (this->number_of_keys_down >= 1)
    {
        this->number_of_keys_down--;
    }
    
    //qDebug() << "keyUp:   "+this->label+" "+ QString::number(keycode);
    this->pc->keyUp(keycode);
}
*/
void Orgelwerk::keyDownRaw(int keycode)
{
    this->pc->keyDownRaw(keycode);
}
void Orgelwerk::keyUpRaw(int keycode)
{
    this->pc->keyUpRaw(keycode);
}

void Orgelwerk::panicKeyPressed()
{
    if (this->piano)
    {
        this->piano->allKeysUp();
    }
    
    if (this->notes)
    {
        this->notes->allKeysUp();
    }
    
    this->pc->allKeysUp();
    
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(true);
    for (int c=0; c < list_of_channels.length(); c++)
    {
        //this->interface_audio->keyPanicEvent(list_of_channels.at(c)["channel"].toInt());
        int channel = list_of_channels.at(c)["channel"].toInt();
        this->list_of_audio_interfaces.at(list_of_channels.at(c)["interface_index"].toInt())->keyPanicEvent(channel);
    }
    
    this->number_of_keys_down = 0;
}
void Orgelwerk::stopAllPressed()
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(true);
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int channel = list_of_channels.at(c)["channel"].toInt();
        this->list_of_audio_interfaces.at(list_of_channels.at(c)["interface_index"].toInt())->keyStopAllEvent(channel);
    }
}

void Orgelwerk::keyMIDIHelper(int midicode, QString mode)
{
    //qDebug() << "################ " << this->number_of_keys_down;
    
    // -12: The lower full octave on the keyboard is in the midi-range of 12 - 23.
    // For being able to add some even deeper notes to the left.
    // Therefore we get an offset of 12 we have to compensate here.
    int keycode = midicode - 12;
    
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(true);
    
    int master_key_shift = this->key_shift_master->value();
    QList<int> list_of_keys = this->keys->getListOfSelectedKeys();
    for (int k=0; k < list_of_keys.length(); k++)
    {
        for (int c=0; c < list_of_channels.length(); c++)
        {
            int channel = list_of_channels.at(c)["channel"].toInt();
            int interface_index = list_of_channels.at(c)["interface_index"].toInt();
            int key_shift = list_of_channels.at(c)["key_shift"].toInt();
            int key_min = list_of_channels.at(c)["key_min"].toInt();
            int key_max = list_of_channels.at(c)["key_max"].toInt();
            int tremolo = list_of_channels.at(c)["tremolo"].toInt();
            
            int m_code = keycode + list_of_keys.at(k);
            int m_code_shifted = m_code + key_shift + master_key_shift;
            
            if ((key_min <= m_code && key_max >= m_code) || mode == "pitch")
            {
                if (mode == "down")
                {
                    if (tremolo == 0)
                    {
                        this->list_of_audio_interfaces.at(interface_index)->keyPressEvent(channel, m_code_shifted);
                        
                        
                    }
                    else
                    {
                        QString worker_code = QString::number(channel)+"_"+QString::number(m_code_shifted);
                        if (!this->map_of_tremolo_threads.contains(worker_code))
                        {
                            TremoloWorker *worker = new TremoloWorker(
                                        interface_index,
                                        tremolo,
                                        channel,
                                        m_code_shifted
                                        );
                            connect(worker, &TremoloWorker::notePlay, this, &Orgelwerk::notePlay);
                            connect(worker, &TremoloWorker::noteStop, this, &Orgelwerk::noteStop);
                            
                            QThread *thread = new QThread(this);
                            worker->moveToThread(thread);
                            thread->start();
                            
                            this->map_of_tremolo_threads[worker_code] = thread;
                        }
                    }
                    
                    if (this->notes) 
                    {
                        this->notes->keyPressed(m_code_shifted);
                    }
                }
                else if (mode == "up")
                {
                    this->list_of_audio_interfaces.at(interface_index)->keyReleaseEvent(channel, m_code_shifted);
                    
                    QString worker_code = QString::number(channel)+"_"+QString::number(m_code_shifted);
                    if (this->map_of_tremolo_threads.contains(worker_code))
                    {
                        this->map_of_tremolo_threads[worker_code]->quit();
                        this->map_of_tremolo_threads[worker_code]->deleteLater();
                        this->map_of_tremolo_threads.remove(worker_code);
                    }
                    
                    if (this->notes)
                    {
                        this->notes->keyReleased(m_code_shifted);
                    }
                }
                else if (mode == "pitch")
                {
                    this->list_of_audio_interfaces.at(interface_index)->keyPitchbendEvent(channel, midicode);
                }
            }
        }
    }
    
    if (mode == "down")
    {
        if (this->piano)
        {
            this->piano->keyPressed(midicode);
        }
    }
    else if (mode == "up")
    {
        if (this->piano)
        {
            this->piano->keyReleased(midicode);
        }
    }
}
void Orgelwerk::keyMIDIDown(int midicode)
{
    keyMIDIHelper(midicode, "down");
}
void Orgelwerk::keyMIDIUp(int midicode)
{
    keyMIDIHelper(midicode, "up");
}

void Orgelwerk::notePlay(int interface_index, int channel, int note)
{
    this->list_of_audio_interfaces.at(interface_index)->keyPressEvent(channel, note);
}
void Orgelwerk::noteStop(int interface_index, int channel, int note)
{
    this->list_of_audio_interfaces.at(interface_index)->keyReleaseEvent(channel, note);
}

// to move pitch wheel by keyboard
void Orgelwerk::movePitchWheel(int key)
{
    this->pitch->movePitchWheel(key);
}
void Orgelwerk::pitchWheelMoved(int pitch)
{
    keyMIDIHelper(pitch, "pitch");
}

void Orgelwerk::keySustain(bool pressed)
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels();
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int interface_index = list_of_channels.at(c)["interface_index"].toInt();
        int channel = list_of_channels.at(c)["channel"].toInt();
        this->list_of_audio_interfaces.at(interface_index)->keySustainEvent(channel, pressed);
    }
}

void Orgelwerk::keySostenuto(bool pressed)
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels();
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int interface_index = list_of_channels.at(c)["interface_index"].toInt();
        int channel = list_of_channels.at(c)["channel"].toInt();
        this->list_of_audio_interfaces.at(interface_index)->keySostenutoEvent(channel, pressed);
    }
}

void Orgelwerk::keySoft(bool pressed)
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(true);
    for (int c=0; c < list_of_channels.length(); c++)
    {
        int interface_index = list_of_channels.at(c)["interface_index"].toInt();
        int channel = list_of_channels.at(c)["channel"].toInt();
        int volume = list_of_channels.at(c)["volume"].toInt();
        this->list_of_audio_interfaces.at(interface_index)->keySoftEvent(channel, pressed, volume);
    }
}

void Orgelwerk::volumeSliderMoved(int value)
{
    this->channels->volumeDCAChanged(value);
}

void Orgelwerk::showChannelDetails()
{
    this->channels->show();
}

void Orgelwerk::channelsDialogRejected()
{
    channelsSummaryUpdate();
}

void Orgelwerk::resendMIDIControls()
{
    this->channels->resendMIDIControls();
}

QList<QMap<QString,QVariant>> Orgelwerk::listOfChannels(bool only_activated)
{
    QList<QMap<QString,QVariant>> list_of_channels = this->channels->listOfChannels(only_activated);
    return list_of_channels;
}
QMap<QString,QVariant> Orgelwerk::getParams()
{
    QMap<QString,QVariant> result;
    
    result["key_shift"] = this->key_shift_master->value();
    result["volume"] = this->volume->getValue();
    result["keys"] = this->keys->getBitmaskOfKeys();
    
    QMap<QString,int> pitchwheel = this->pitch->getData();
    
    //for (int i=0; i < pitchwheel.keys().length(); i++)
    for (int i=0; i < pitchwheel.count(); i++)
    {
        //result[pitchwheel.keys().at(i)] = pitchwheel[pitchwheel.keys().at(i)];
        result[pitchwheel.keys().at(i)] = pitchwheel[pitchwheel.keys().at(i)];
    }
    
    return result;
}
void Orgelwerk::restoreParams(QMap<QString,QVariant> data)
{
    this->channels->restoreParams(data);
    channelsSummaryUpdate();
    
    QMap<QString,QVariant> main = data["main"].toMap();
    this->key_shift_master->setValue(main["key_shift"].toInt());
    this->volume->setValue(main["volume"].toInt());
    this->keys->restoreBitmaskOfKeys(main["keys"].toString());
    this->pitch->setData(main);
}

void Orgelwerk::showHideGUIElements(QString name, bool show)
{
    if (name == "keyshift")
    {
        if (show)
        {
            this->key_shift_master->show();
        }
        else
        {
            this->key_shift_master->hide();
        }
    }
    else if (name == "keys")
    {
        if (show)
            this->group_keys->show();
        else
            this->group_keys->hide();
    }
    else if (name == "pitch")
    {
        if (show)
            this->group_pitch->show();
        else
            this->group_pitch->hide();
    }
    else if (name == "notes")
    {
        if (show)
            this->notes->show();
        else
            this->notes->hide();
    }
    else if (name == "pc")
    {
        if (show)
            this->pc->show();
        else
            this->pc->hide();
    }
    else if (name == "piano")
    {
        if (show)
            this->piano->show();
        else
            this->piano->hide();
    }
}

bool Orgelwerk::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyRelease)
    {
        emit eventFiltered(obj, ev);
        
        return true;
    }
    
    return false;
}

bool Orgelwerk::areKeysPressed()
{
    if (this->number_of_keys_down > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}



TremoloWorker::TremoloWorker(int interface_index, int delay, int channel, int note, QObject *parent)
    : QObject(parent)
{
    this->interface_index = interface_index;
    this->delay = delay;
    this->channel = channel;
    this->note = note;
    
    this->timer = new QTimer(this);
    this->timer->setInterval(delay);
    this->timer->setTimerType(Qt::PreciseTimer);
    
    connect(this->timer, &QTimer::timeout, this, &TremoloWorker::tick, Qt::DirectConnection);
    
    this->timer->start();
}

void TremoloWorker::tick()
{
    emit noteStop(this->interface_index, this->channel, this->note);
    emit notePlay(this->interface_index, this->channel, this->note);
    
    // make the tremolo effect feeling a bit more natural by not having all the time the exact same delay but just modify it a little bit
    int rnd = this->random->bounded(
                0,
                int(this->delay * .5)
                );
    int interval = this->delay + rnd;
    this->timer->setInterval(interval);
}
