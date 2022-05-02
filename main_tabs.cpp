#include "main_tabs.h"

MainTabs::MainTabs(int id, Config *config, QString output_system, QComboBox *combo_keyboard_input, QPushButton *button_lock, QPushButton *button_keyboard_rescan, QLineEdit *line_udp_ip, QSpinBox *spin_port, QTabWidget *parent) : QTabWidget(parent)
{
    this->id = id;
    this->config = config;
    if (output_system == "network")
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
    
    if (output_system != "network")
    {
        connect(line_udp_ip, &QLineEdit::textChanged, this, &MainTabs::rebindSocketIP);
        connect(spin_port, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainTabs::rebindSocket);
        
        socket->bind(QHostAddress::LocalHost, spin_port->value());
        connect(socket, &QUdpSocket::readyRead, this, &MainTabs::receiveUDPMessage);
        
        initializeTabs(output_system);
    }
    else
    {
        initializeTabs(output_system);
        
        hide();
    }
    
    installEventFilter(this);
}

void MainTabs::initializeTabs(QString output_system)
{
    this->list_function_keys = {Qt::Key_F1, Qt::Key_F2, Qt::Key_F3, Qt::Key_F4, -1, Qt::Key_F5, Qt::Key_F6, Qt::Key_F7, Qt::Key_F8, -1, Qt::Key_F9, Qt::Key_F10, Qt::Key_F11, Qt::Key_F12};
    this->list_function_keys_raw = {59, 60, 61, 62, -1, 63, 64, 65, 66, -1, 67, 68, 87, 88};
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
            addOrganTab(output_system, this->list_labels.at(i), 1);
        }
    }
}

void MainTabs::addOrganTab(QString output_system, QString label, int number_of_orgelwerks)
{
    /*
    Orgelwerk *o = new Orgelwerk(label);
    addTab(o, label);
    */
    
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
        
        Orgelwerk *o = new Orgelwerk(this->id, output_system, midi_port_label);
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
    Q_UNUSED(obj);
    
    if (ev->type() == QEvent::KeyPress)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(ev);
        
        if (!event->isAutoRepeat())
        {
            if (event->key() == Qt::Key_Control)
            {
                this->ctrl_down = true;
            }
            else if (event->key() == Qt::Key_Shift)
            {
                if (this->ctrl_down)
                {
                    qDebug() << "ctr shift";
                    toggleKeyboardLock();
                    
                    return true;
                }
            }
            
            //Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget());
            Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget()->layout()->itemAt(0)->widget());
            
            QString udp_message = QString::number(ev->type())+"/"+QString::number(event->key());
            
            if (event->key() == Qt::Key_Escape)
            {
                //o->button_panic->animateClick();
                for (int i=0; i < this->list_of_tabs.length(); i++)
                {
                    this->list_of_tabs.at(i)->button_panic->animateClick();
                }
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Delete)
            {
                //o->button_stop_all->animateClick();
                for (int i=0; i < this->list_of_tabs.length(); i++)
                {
                    this->list_of_tabs.at(i)->button_stop_all->animateClick();
                }
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Menu)
            {
                o->button_channels_dialog->animateClick();
                sendUDPMessage(udp_message);
            }
            else if (event->key() == Qt::Key_Insert)
            {
                o->button_resend_midi->animateClick();
                sendUDPMessage(udp_message);
            }
            else if (event->key() == Qt::Key_Space)
            {
                o->keySustain(true);
                o->keyDown(Qt::Key_Space);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Alt)
            {
                o->keySostenuto(true);
                o->keyDown(Qt::Key_Alt);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Super_L || event->key() == Qt::Key_Super_R)
            {
                o->keySoft(true);
                o->keyDown(Qt::Key_Super_L);
                o->keyDown(Qt::Key_Super_R);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            // activate desired tab by pressing an f-key
            else if (this->list_function_keys.contains(event->key()))
            {
                for (int i=0; i < this->list_function_keys.length(); i++)
                {
                    if (this->list_function_keys.at(i) == event->key())
                    {
                        // send "all keys of" on old tab
                        //o->panicKeyPressed();
                        
                        // activate new tab
                        this->setCurrentIndex(i);
                        
                        // current widget changed, need to update
                        //o = static_cast<Orgelwerk*>(currentWidget()->layout()->itemAt(0)->widget());
                        //o->resendMIDIControls();
                        
                        sendUDPMessage(udp_message);
                        
                        return true;
                    }
                }
            }
            // use the arrow-keys to operate the pitch-wheel
            else if ( (event->key() == Qt::Key_Left) | (event->key() == Qt::Key_Right) )
            //else if (event->key() == Qt::Key_Up | event->key() == Qt::Key_Down)
            {
                //o->movePitchWheel(event->key());
                o->pitch->pitchKeyPressed(event->key());
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Up | event->key() == Qt::Key_Down | event->key() == Qt::Key_PageUp | event->key() == Qt::Key_PageDown)
            {
                o->volume->volumeKeyPressed(event->key());
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            // input for the virtual keyboard(s)
            else
            {
                o->keyDown(event->key());
                
                sendUDPMessage(udp_message);
                
                return true;
            }
        }
    }
    else if (ev->type() == QEvent::KeyRelease)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(ev);
        
        if (!event->isAutoRepeat())
        {
            //Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget());
            Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget()->layout()->itemAt(0)->widget());
            
            QString udp_message = QString::number(ev->type())+"/"+QString::number(event->key());
            
            if (event->key() == Qt::Key_Space)
            {
                o->keySustain(false);
                o->keyUp(Qt::Key_Space);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Alt)
            {
                o->keySostenuto(false);
                o->keyUp(Qt::Key_Alt);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Super_L || event->key() == Qt::Key_Super_R)
            {
                o->keySoft(false);
                o->keyUp(Qt::Key_Super_L);
                o->keyUp(Qt::Key_Super_R);
                
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Left | event->key() == Qt::Key_Right)
            {
                o->pitch->pitchKeyReleased();
                sendUDPMessage(udp_message);
                
                return true;
            }
            else if (event->key() == Qt::Key_Up | event->key() == Qt::Key_Down | event->key() == Qt::Key_PageUp | event->key() == Qt::Key_PageDown)
            {
                o->volume->volumeKeyReleased();
                sendUDPMessage(udp_message);
                
                return true;
            }
            else
            {
                // this function should be kept just in case "areKeysPressed" would return "false" falsely on current tab ...
                //o->keyUp(event->key());
                // we want to have a smooth way of switching between tabs (=presets) during playing
                
                for (int i=0; i < this->list_of_tabs.length(); i++)
                {
                    //if (this->list_of_tabs.at(i)->areKeysPressed())
                    //{
                        this->list_of_tabs.at(i)->keyUp(event->key());
                    //}
                }
                
                sendUDPMessage(udp_message);
                
                return true;
            }
        }
    }
    else if (ev->type() == QEvent::MouseButtonPress)
    {
        if (this->keyboard_locked)
        {
            toggleKeyboardLock();
        }
    }
    
    return false;
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
    QString devpath = this->keyboard_raw->getPathForName(text);
    
    if (this->keyboard_locked)
    {
        toggleKeyboardLock();
    }
    /*
    {
        this->keyboard_raw->keyboardLock(devpath);
    }
    else
    {
        this->keyboard_raw->keyboardListen(devpath);
    }
    */
}
void MainTabs::deviceNotAvailable(QString message)
{
    qDebug() << "main_tabs: deviceNotAvailable";
}
void MainTabs::rawKeyPressed(int keycode)
{
    qDebug() << "main_tabs: rawKeyPressed: " << keycode;
    
    for (int i=0; i < this->list_function_keys.length(); i++)
    {
        if (this->list_function_keys_raw.at(i) == keycode)
        {
            // activate new tab
            this->setCurrentIndex(i);
        }
    }
    
    if (keycode == 1) // escape
    {
        for (int i=0; i < this->list_of_tabs.length(); i++)
        {
            this->list_of_tabs.at(i)->button_panic->animateClick();
        }
    }
    else if (keycode == 110) // insert
    {
        Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget()->layout()->itemAt(0)->widget());
        o->button_resend_midi->animateClick();
    }
    else if (keycode == 111) // delete
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
void MainTabs::toggleKeyboardLock()
{
    if (this->keyboard_locked)
    {
        this->keyboard_locked = false;
        this->button_lock->setDown(false);
        this->button_lock->setText("Lock");
        
        this->combo_keyboard_input->setEnabled(true);
        
        if (this->combo_keyboard_input->currentIndex() == 0)
        {
            qDebug() << "releasing";
            releaseKeyboard();
            releaseMouse();
        }
        else
        {
            this->keyboard_raw->keyboardRelease();
        }
    }
    else
    {
        this->keyboard_locked = true;
        this->button_lock->setDown(true);
        this->button_lock->setText("Unlock");
        
        this->combo_keyboard_input->setEnabled(false);
        
        if (this->combo_keyboard_input->currentIndex() == 0)
        {
            qDebug() << "grabbing";
            grabKeyboard();
            // to avoid to mess around with the os like blocking taskbar-items, we need to grab the mouse aswell
            grabMouse();
        }
        else
        {
            //qDebug() << "locking: release";
            //this->keyboard_raw->keyboardRelease();
            qDebug() << "locking: lock";
            QString devpath = this->keyboard_raw->getPathForName(this->combo_keyboard_input->currentText());
            this->keyboard_raw->keyboardLock(devpath);
        }
    }
}

void MainTabs::keyboardRescan()
{
    if (this->keyboard_locked)
        toggleKeyboardLock();
    
    InputKeyboardRaw *keyboard_raw = new InputKeyboardRaw;
    QList<QString> keyboards = keyboard_raw->getKeyboardNames();
    keyboards.prepend("generic default");
    this->combo_keyboard_input->blockSignals(true);
    this->combo_keyboard_input->clear();
    this->combo_keyboard_input->addItems(keyboards);
    this->combo_keyboard_input->blockSignals(false);
}
