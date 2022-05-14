#include "main_tabs.h"

MainTabs::MainTabs(int id, Config *config, OutputSystem output, QComboBox *combo_keyboard_input, QPushButton *button_lock, QPushButton *button_keyboard_rescan, QLineEdit *line_udp_ip, QSpinBox *spin_port, QTabWidget *parent) : QTabWidget(parent)
{
    this->id = id;
    this->config = config;
    if (output == OutputSystem::Network)
    {
        this->send_udp = true;
    }
    this->combo_keyboard_input = combo_keyboard_input;
    connect(this->combo_keyboard_input, &QComboBox::currentTextChanged, this, &MainTabs::keyboardSelectionChanged);
    this->keyboard_raw = new InputKeyboardRaw;
    connect(this->keyboard_raw, &InputKeyboardRaw::deviceNotAvailable, this, &MainTabs::deviceNotAvailable);
    connect(this->keyboard_raw, &InputKeyboardRaw::rawKeyPressedSignal, this, &MainTabs::rawKeyPressed);
    connect(this->keyboard_raw, &InputKeyboardRaw::rawKeyReleasedSignal, this, &MainTabs::rawKeyReleased);
    
    this->button_lock = button_lock;
    connect(this->button_lock, &QPushButton::clicked, this, &MainTabs::toggleKeyboardLock);
    connect(button_keyboard_rescan, &QPushButton::clicked, this, &MainTabs::keyboardRescan);
    keyboardRescan();
    this->line_udp_ip = line_udp_ip;
    this->spin_port = spin_port;
    
    setMovable(false);
    
    this->socket = new QUdpSocket(this);
    
    if (output != OutputSystem::Network)
    {
        connect(line_udp_ip, &QLineEdit::textChanged, this, &MainTabs::rebindSocketIP);
        connect(spin_port, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainTabs::rebindSocket);
        
        socket->bind(QHostAddress::LocalHost, spin_port->value());
        connect(socket, &QUdpSocket::readyRead, this, &MainTabs::receiveUDPMessage);
        
        initializeTabs(output);
    }
    else
    {
        initializeTabs(output);
        
        hide();
    }
    
    //installEventFilter(this);
    
}

void MainTabs::initializeTabs(OutputSystem output)
{
    //this->list_function_keys = {Qt::Key_F1, Qt::Key_F2, Qt::Key_F3, Qt::Key_F4, -1, Qt::Key_F5, Qt::Key_F6, Qt::Key_F7, Qt::Key_F8, -1, Qt::Key_F9, Qt::Key_F10, Qt::Key_F11, Qt::Key_F12};
    this->list_function_keys_raw = {KeysRaw::F1, KeysRaw::F2, KeysRaw::F3, KeysRaw::F4, -1, KeysRaw::F5, KeysRaw::F6, KeysRaw::F7, KeysRaw::F8, -1, KeysRaw::F9, KeysRaw::F10, KeysRaw::F11, KeysRaw::F12};
    //this->list_function_keys_native = {67, 68, 69, 70, -1, 71, 72, 73, 74, -1, 75, 76, 95, 96};
    this->list_labels = {"F1", "F2", "F3", "F4", "spacer", "F5", "F6", "F7", "F8", "spacer", "F9", "F10", "F11", "F12"};
    
    int number_of_tabs = this->list_labels.length();
    //number_of_tabs = 4;
    for (int i=0; i < number_of_tabs; i++)
    {
        if (this->list_labels.at(i) == "spacer")
        {
            // as a "spacer" we just create an invisible tab with given width
            // first we create a hidden tab and than style hidden tabs to be invisible
            QWidget *w = new QWidget;
            int index = addTab(w, "");
            
            setTabEnabled(index, false);
        }
        else
        {
            addOrganTab(output, this->list_labels.at(i), 1);
        }
    }
}

void MainTabs::addOrganTab(OutputSystem output, QString label, int number_of_orgelwerks)
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    widget->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    
    QString midi_port_label = label;
    for (int i=0; i < number_of_orgelwerks; i++)
    {
        if (number_of_orgelwerks > 1)
        {
            midi_port_label = label + "-" + QString::number(i);
        }
        
        Orgelwerk *o = new Orgelwerk(this->id, output, midi_port_label);
        connect(o, &Orgelwerk::eventFiltered, this, &MainTabs::eventFilter);
        
        layout->addWidget(o);
        
        this->list_of_tabs.append(o);
        this->map_of_tabs[label] = o;
    }
    
    addTab(widget, label);
}

void MainTabs::saveAllParams()
{
    for (int i=0; i < count(); i++)
    {
        QString label = tabText(i);
        if (! label.isEmpty())
        {
            Orgelwerk *o = static_cast<Orgelwerk*>(widget(i)->layout()->itemAt(0)->widget());
            
            QList<QMap<QString,QVariant>> channels = o->listOfChannels(false);
            this->config->saveChannelSettings(this->id, label, channels);
            
            QMap<QString,QVariant> params = o->getParams();
            this->config->saveParams(this->id, label, "main", params);
        }
    }
    
    QMap<QString,QVariant> params;
    params["network_ip"] = this->line_udp_ip->text();
    params["network_port"] = this->spin_port->value();
    
    this->config->saveParams(this->id, "general", "", params);
}
void MainTabs::restoreParams(QString tab, QMap<QString,QVariant> data)
{
    this->map_of_tabs[tab]->restoreParams(data);
}

void MainTabs::showHideGUIElements(QString name, bool show)
{
    for (int i=0; i < count(); i++)
    {
        QString label = tabText(i);
        if (! label.isEmpty())
        {
            Orgelwerk *o = static_cast<Orgelwerk*>(widget(i)->layout()->itemAt(0)->widget());
            o->showHideGUIElements(name, show);
        }
    }
    resize(10, 10);
}

bool MainTabs::callEventFilter(QObject *obj, QEvent *ev)
{
    return eventFilter(obj, ev);
}
bool MainTabs::eventFilter(QObject *obj, QEvent *ev)
{
    if (! this->input_kbd_qt_default)
    {
        return false;
    }
    
    //return this->keyboard_qt->callEventFilter(obj, ev);
}

void MainTabs::sendUDPMessage(QString message)
{
    QByteArray udp_message;
    udp_message = message.toUtf8();
    
    if (this->send_udp)
    {
        this->socket->writeDatagram(udp_message, QHostAddress(this->line_udp_ip->text()), this->spin_port->value());
    }
}

void MainTabs::receiveUDPMessage()
{
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());
    
    QHostAddress sender;
    quint16 senderPort;
    
    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);
    
    QString input = QString(buffer);
    QStringList splitted = input.split("/");
    
    int type = splitted.at(0).toInt();
    int key = splitted.at(1).toInt();
    
    if (type == QEvent::KeyPress)
    {
        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier);
        QCoreApplication::postEvent(this, event);
    }
    else if (type == QEvent::KeyRelease)
    {
        QKeyEvent *event = new QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);
        QCoreApplication::postEvent(this, event);
    }
}

void MainTabs::rebindSocketIP(QString ip)
{
    socket->close();
    socket->bind(QHostAddress(ip), this->spin_port->value());
}
void MainTabs::rebindSocket(int value)
{
    socket->close();
    socket->bind(QHostAddress(this->line_udp_ip->text()), value);
}

void MainTabs::keyboardSelectionChanged(QString text)
{
    if (this->keyboard_locked)
    {
        toggleKeyboardLock();
    }
    
    // unlock the keyboard if selection changed and it was locked (probably this is not possible anymore anyways since the lock-button deactivates this->combo_keyboard_input, but it is better to leave this code in here, just in case ...)
    this->keyboard_raw->keyboardRelease();
    
    //if (this->combo_keyboard_input->currentIndex() == 0)
    if (this->combo_keyboard_input->currentText() == this->combo_keyboard_input_labels[0])
    {
        emit useInputKbdQtNativeSignal();
    }
    else if (this->combo_keyboard_input->currentText() == this->combo_keyboard_input_labels[1])
    {
        emit useInputKbdQtDefaultSignal();
    }
    else
    {
        QString devpath = this->keyboard_raw->getPathForName(text);
        
        this->keyboard_raw->keyboardListen(devpath);
    }
}
void MainTabs::deviceNotAvailable(QString message)
{
    qDebug() << "main_tabs: deviceNotAvailable";
}
void MainTabs::rawKeyPressed(int keycode)
{
    qDebug() << "main_tabs: rawKeyPressed: " << keycode;
    
    for (int i=0; i < this->list_function_keys_raw.length(); i++)
    {
        if (this->list_function_keys_raw.at(i) == keycode)
        {
            // activate new tab
            this->setCurrentIndex(i);
        }
    }
    
    if (keycode == KeysRaw::Escape)
    {
        for (int i=0; i < this->list_of_tabs.length(); i++)
        {
            this->list_of_tabs.at(i)->button_panic->animateClick();
        }
    }
    else if (keycode == KeysRaw::Insert)
    {
        Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget()->layout()->itemAt(0)->widget());
        o->button_resend_midi->animateClick();
    }
    else if (keycode == KeysRaw::Delete)
    {
        for (int i=0; i < this->list_of_tabs.length(); i++)
        {
            this->list_of_tabs.at(i)->button_stop_all->animateClick();
        }
    }
    else
    {
        Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget()->layout()->itemAt(0)->widget());
        o->keyDownRaw(keycode);
    }
}
void MainTabs::rawKeyReleased(int keycode)
{
    qDebug() << "main_tabs: rawKeyReleased: " << keycode;
    
    // we want to have a smooth way of switching between tabs (=presets) during playing
    for (int i=0; i < this->list_of_tabs.length(); i++)
    {
        this->list_of_tabs.at(i)->keyUpRaw(keycode);
    }
}
void MainTabs::changeTab(int id){
    this->setCurrentIndex(id);
}
void MainTabs::MIDISignal(MIDISignalTypes type)
{
    Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget()->layout()->itemAt(0)->widget());
    
    if (type == MIDISignalTypes::ToggleKeyboardLock)
    {
        
    }
    else if (type == MIDISignalTypes::Panick)
    {
        for (int i=0; i < this->list_of_tabs.length(); i++)
        {
            this->list_of_tabs.at(i)->button_panic->animateClick();
        }
    }
    else if (type == MIDISignalTypes::StopAll)
    {
        for (int i=0; i < this->list_of_tabs.length(); i++)
        {
            this->list_of_tabs.at(i)->button_stop_all->animateClick();
        }
    }
    else if (type == MIDISignalTypes::ShowMenu)
    {
        o->button_channels_dialog->animateClick();
    }
    else if (type == MIDISignalTypes::ResendMIDISettings)
    {
        o->button_resend_midi->animateClick();
    }
    else if (type == MIDISignalTypes::SustainPressed)
    {
        o->keySustain(true);
        o->keyDown(Qt::Key_Space);
    }
    else if (type == MIDISignalTypes::SustainReleased)
    {
        o->keySustain(false);
        o->keyUp(Qt::Key_Space);
    }
    else if (type == MIDISignalTypes::SostenutoPressed)
    {
        o->keySostenuto(true);
        o->keyDown(Qt::Key_Alt);
    }
    else if (type == MIDISignalTypes::SostenutoReleased)
    {
        o->keySostenuto(false);
        o->keyUp(Qt::Key_Alt);
    }
    else if (type == MIDISignalTypes::SoftPressed)
    {
        o->keySoft(true);
        o->keyDown(Qt::Key_Super_L);
        o->keyDown(Qt::Key_Super_R);
    }
    else if (type == MIDISignalTypes::SoftReleased)
    {
        o->keySoft(false);
        o->keyUp(Qt::Key_Super_L);
        o->keyUp(Qt::Key_Super_R);
    }
    else if (type == MIDISignalTypes::VolumeLowerPressed)
    {
        o->volume->volumeKeyPressed(Qt::Key_Down);
    }
    else if (type == MIDISignalTypes::VolumeHigherPressed)
    {
        o->volume->volumeKeyPressed(Qt::Key_Up);
    }
    else if (type == MIDISignalTypes::VolumeReleased)
    {
        o->volume->volumeKeyReleased();
    }
    else if (type == MIDISignalTypes::PitchLowerPressed)
    {
        o->pitch->pitchKeyPressed(Qt::Key_Left);
    }
    else if (type == MIDISignalTypes::PitchHigherPressed)
    {
        o->pitch->pitchKeyPressed(Qt::Key_Right);
    }
    else if (type == MIDISignalTypes::PitchReleased)
    {
        o->pitch->pitchKeyReleased();
    }
}

void MainTabs::toggleKeyboardLock()
{
    //this->keyboard_raw->keyboardRelease();
    
    if (this->keyboard_locked)
    {
        this->keyboard_locked = false;
        this->button_lock->setDown(false);
        this->button_lock->setText("Lock");
        
        this->combo_keyboard_input->setEnabled(true);
        
        if (this->combo_keyboard_input->currentText() == this->combo_keyboard_input_labels[1])
        {
            this->input_kbd_qt_default = false;
            qDebug() << "releasing Qt default";
            releaseKeyboard();
            //releaseMouse();
        }
        else if (this->combo_keyboard_input->currentText() == this->combo_keyboard_input_labels[0])
        {
            this->input_kbd_qt_native = false;
            qDebug() << "releasing Qt native";
            releaseKeyboard();
        }
        else
        {
            this->input_kbd_linux_raw = false;
            qDebug() << "releasing raw";
            this->keyboard_raw->keyboardRelease();
        }
    }
    else
    {
        this->keyboard_locked = true;
        this->button_lock->setDown(true);
        this->button_lock->setText("Unlock");
        
        this->combo_keyboard_input->setEnabled(false);
        
        if (this->combo_keyboard_input->currentText() == this->combo_keyboard_input_labels[1])
        {
            this->input_kbd_qt_default = true;
            qDebug() << "grabbing Qt default";
            grabKeyboard();
            // to avoid to mess around with the os like blocking taskbar-items, we need to grab the mouse aswell
            //grabMouse();
        }
        else if (this->combo_keyboard_input->currentText() == this->combo_keyboard_input_labels[0])
        {
            this->input_kbd_qt_native = true;
            qDebug() << "grabbing Qt native";
            grabKeyboard();
        }
        else
        {
            this->input_kbd_linux_raw = true;
            //qDebug() << "locking: release";
            //this->keyboard_raw->keyboardRelease();
            qDebug() << "gabbing: raw";
            QString devpath = this->keyboard_raw->getPathForName(this->combo_keyboard_input->currentText());
            this->keyboard_raw->keyboardLock(devpath);
        }
    }
}

void MainTabs::keyboardRescan()
{
    if (this->keyboard_locked)
        toggleKeyboardLock();
    
    this->combo_keyboard_input_labels[0] = "Qt native";
    this->combo_keyboard_input_labels[1] = "Qt default";
    
    InputKeyboardRaw *keyboard_raw = new InputKeyboardRaw;
    QList<QString> keyboards = keyboard_raw->getKeyboardNames();
    for (int i=this->combo_keyboard_input_labels.size()-1; i >= 0; i--)
    {
        keyboards.prepend(this->combo_keyboard_input_labels[i]);
    }
    this->combo_keyboard_input->blockSignals(true);
    this->combo_keyboard_input->clear();
    this->combo_keyboard_input->addItems(keyboards);
    this->combo_keyboard_input->blockSignals(false);
}
