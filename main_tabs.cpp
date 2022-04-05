#include "main_tabs.h"

MainTabs::MainTabs(Config *config, QString mode, QLineEdit *line_udp_ip, QSpinBox *spin_port, QTabWidget *parent) : QTabWidget(parent)
{
    this->config = config;
    this->mode = mode;
    this->line_udp_ip = line_udp_ip;
    this->spin_port = spin_port;
    
    setMovable(false);
    
    this->socket = new QUdpSocket(this);
    
    if (mode == "default")
    {
        connect(line_udp_ip, &QLineEdit::textChanged, this, &MainTabs::rebindSocketIP);
        connect(spin_port, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainTabs::rebindSocket);
        
        socket->bind(QHostAddress::LocalHost, spin_port->value());
        connect(socket, &QUdpSocket::readyRead, this, &MainTabs::receiveUDPMessage);
        
        initializeTabs();
    }
    else
    {
        initializeTabs();
        
        hide();
    }
    
    installEventFilter(this);
}

void MainTabs::initializeTabs()
{
    this->list_function_keys = {Qt::Key_F1, Qt::Key_F2, Qt::Key_F3, Qt::Key_F4, -1, Qt::Key_F5, Qt::Key_F6, Qt::Key_F7, Qt::Key_F8, -1, Qt::Key_F9, Qt::Key_F10, Qt::Key_F11, Qt::Key_F12};
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
            addOrganTab(this->list_labels.at(i), 1);
        }
    }
}

void MainTabs::addOrganTab(QString label, int number_of_orgelwerks)
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
        
        Orgelwerk *o = new Orgelwerk(midi_port_label);
        connect(o, &Orgelwerk::eventFiltered, this, &MainTabs::eventFilter);
        
        layout->addWidget(o);
        
        this->list_of_tabs.append(o);
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
            this->config->saveChannelSettings(label, channels);
        }
    }
}
void MainTabs::openAllParams()
{
    for (int i=0; i < count(); i++)
    //for (int i=0; i < 1; i++)
    {
        QString label = tabText(i);
        if (! label.isEmpty())
        {
            Orgelwerk *o = static_cast<Orgelwerk*>(widget(i)->layout()->itemAt(0)->widget());
            
            QList<QMap<QString,QVariant>> channels = this->config->loadChannelSettings(label);
            o->setListOfChannels(channels);
        }
    }
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
    
    return false;
}

void MainTabs::sendUDPMessage(QString message)
{
    QByteArray udp_message;
    udp_message = message.toUtf8();
    
    if (this->mode == "udp_client")
    {
        //this->socket->writeDatagram(udp_message, QHostAddress::LocalHost, 20002);
        this->socket->writeDatagram(udp_message, QHostAddress(this->line_udp_ip->text()), this->spin_port->value());
        //this->socket->writeDatagram(udp_message, QHostAddress("127.0.0.1"), 20002);
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
