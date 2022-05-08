#include "main_window.h"

MainWindow::MainWindow(QString output_system, int number_of_keyboards, QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *main_container_widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    main_container_widget->setLayout(layout);
    setCentralWidget(main_container_widget);
    
    QFile css_file(":css_allen_heath");
    if (css_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        setStyleSheet(css_file.readAll());
    }
    
    if (output_system != "network")
    {
        MenuBar *menu = new MenuBar;
        connect(menu, &MenuBar::signalSave, this, &MainWindow::saveAllParams);
        connect(menu, &MenuBar::signalOpen, this, &MainWindow::openAllParams);
        connect(menu, &MenuBar::signalShowActionChanged, this, &MainWindow::showActionChanged);
        setMenuBar(menu);
    }
    
    for (int i=0; i < number_of_keyboards; i++)
    {
        layout->addWidget(newKeyboardInstance(i, output_system));
    }
}

MainWindow::~MainWindow()
{
    
}

QWidget* MainWindow::newKeyboardInstance(int id, QString mode)
{
    QWidget *widget = new QWidget;
    //QVBoxLayout *layout = new QVBoxLayout;
    QGridLayout *grid = new QGridLayout;
    //widget->setLayout(layout);
    widget->setLayout(grid);
    
    this->config = new Config;
    connect(this->config, &Config::restoreParams, this, &MainWindow::restoreParams);
    connect(this->config, &Config::restoreGeneral, this, &MainWindow::restoreGeneral);
    
    QPushButton *button_lock = new QPushButton("Lock");
    button_lock->setObjectName("button_grab");
    
    QPushButton *button_keyboard_rescan = new QPushButton();
    button_keyboard_rescan->setIcon(QIcon::fromTheme("system-reboot"));
    button_keyboard_rescan->setToolTip("Rescan Keyboards");
    
    QComboBox *combo_keyboard_input = new QComboBox();
    
    QLineEdit *line_udp_ip = new QLineEdit(this);
    line_udp_ip->setText("127.0.0.1");
    line_udp_ip->setToolTip("For remote-controlling: IP-Address of the interface you want to listen on (the IP-address of this machine).");
    line_udp_ip->setObjectName("network_select");
    //line_udp_ip->hide();
    this->list_of_line_udp_ips.append(line_udp_ip);
    
    QSpinBox *spin_port = new QSpinBox;
    spin_port->setMinimum(1025);
    spin_port->setMaximum(65535);
    spin_port->setValue(20020);
    spin_port->setToolTip("Network Listen Port");
    spin_port->setObjectName("network_select");
    //spin_port->hide();
    this->list_of_spin_ports.append(spin_port);
    
    MainTabs *tabs = new MainTabs(id, this->config, mode, combo_keyboard_input, button_lock, button_keyboard_rescan, line_udp_ip, spin_port);
    this->list_of_maintabs.append(tabs);
    
    QPushButton *button_lock_help = new QPushButton;
    button_lock_help->setIcon(QIcon::fromTheme("dialog-question"));
    button_lock_help->setToolTip("help");
    button_lock_help->hide();
    
    QPushButton *button_network_help = new QPushButton;
    button_network_help->setIcon(QIcon::fromTheme("dialog-question"));
    button_network_help->setToolTip("help");
    connect(button_network_help, &QPushButton::clicked, this, []{ new HelpMessage(":help_tntware"); });
    button_network_help->hide();
    this->list_of_network_help_buttons.append(button_network_help);
    
    grid->addWidget(combo_keyboard_input, 1, 0);
    grid->addWidget(button_lock, 1, 1);
    grid->addWidget(button_keyboard_rescan, 1, 2);
    //grid->addWidget(button_lock_help, 1, 3);
    if (mode == "network")
    {
        QLabel *label_udp_client_ip = new QLabel("IP-Address of remote vkeybd-qt instance:");
        grid->addWidget(label_udp_client_ip, 2, 0);
        
        QLabel *label_udp_client_port = new QLabel("Port:");
        grid->addWidget(label_udp_client_port, 2, 1);
    }
    else
    {
        line_udp_ip->hide();
        spin_port->hide();
    }
    grid->addWidget(line_udp_ip, 3, 0);
    grid->addWidget(spin_port, 3, 1, 1, 2);
    if (mode != "network")
    {
        grid->addWidget(tabs, 4, 0, 1, 3);
    }
    
    //this->installEventFilter(this);
    //QCoreApplication::installNativeEventFilter(this);
    //QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
    //QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
    
    QAbstractNativeEventFilter *xev = new InputNativeEventFilter;
    QAbstractEventDispatcher::instance()->installNativeEventFilter(xev);
    
    //int width = this->width();
    //resize(width, 900);
    //resize(10, 10);
    
    qDebug() << "native parent: " << nativeParentWidget();
    
    return widget;
}

void MainWindow::saveAllParams()
{
    for (int i=0; i < this->list_of_maintabs.length(); i++)
    {
        this->list_of_maintabs.at(i)->saveAllParams();
    }
}
void MainWindow::openAllParams()
{
    this->config->loadChannelSettings();
}
void MainWindow::restoreParams(int maintab, QString tab, QMap<QString, QVariant> data)
{
    if (this->list_of_maintabs.length() >= maintab+1)
    {
        this->list_of_maintabs.at(maintab)->restoreParams(tab, data);
    }
    
}
void MainWindow::restoreGeneral(int maintab, QMap<QString,QVariant> data)
{
    if (this->list_of_maintabs.length() >= maintab+1)
    {
        this->list_of_line_udp_ips.at(maintab)->setText(data["network_ip"].toString());
        this->list_of_spin_ports.at(maintab)->setValue(data["network_port"].toInt());
    }
}

void MainWindow::showActionChanged(QString name, bool is_checked)
{
    if (name == "network")
    {
        for (auto &line : this->list_of_line_udp_ips)
        {
            if (is_checked)
                line->show();
            else
                line->hide();
        }
        for (auto &spin : this->list_of_spin_ports)
        {
            if (is_checked)
                spin->show();
            else
                spin->hide();
        }
        for (auto &button : this->list_of_network_help_buttons)
        {
            if (is_checked)
            {
                //button->show();
            }
            else
            {
                button->hide();
            }
        }
    }
    else
    {
        for (auto &maintab : this->list_of_maintabs)
        {
            maintab->showHideGUIElements(name, is_checked);
        }
    }
    
    //resize(10, 10);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    //return this->list_of_maintabs.first()->callEventFilter(obj, ev);
    /*
    qDebug() << "evfilter";
    for (auto &maintab : this->list_of_maintabs)
    {
        qDebug() << "for";
        //return maintab->callEventFilter(obj, ev);
        
        if (maintab->hasFocus())
        {
            //return maintab->callEventFilter(obj, ev);
            qDebug() << "focus";
        }
        
    }
    */
    
    return false;
}

/*
bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
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
                    if (this->grabbing != nullptr)
                    {
                        grabButtonClicked(this->grabbing);
                    }
                    else if (this->grabbing_last != nullptr)
                    {
                        grabButtonClicked(this->grabbing_last);
                    }
                    else
                    {
                        grabButtonClicked(this->list_of_button_grabs.first());
                    }
                    
                    return true;
                }
            }
            else
            {
                //return this->tabs->callEventFilter(obj, ev);
                
                if (this->grabbing != nullptr)
                {
                    int index = this->list_of_button_grabs.indexOf(this->grabbing);
                    return this->list_of_maintabs.at(index)->callEventFilter(obj, ev);
                }
                else
                {
                    return this->list_of_maintabs.first()->callEventFilter(obj, ev);
                }
            }
        }
    }
    else if (ev->type() == QEvent::KeyRelease)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(ev);
        
        if (!event->isAutoRepeat())
        {
            if (event->key() == Qt::Key_Control)
            {
                this->ctrl_down = false;
            }
            else
            {
                //return this->tabs->callEventFilter(obj, ev);
                
                if (this->grabbing != nullptr)
                {
                    int index = this->list_of_button_grabs.indexOf(this->grabbing);
                    return this->list_of_maintabs.at(index)->callEventFilter(obj, ev);
                }
                else
                {
                    return this->list_of_maintabs.first()->callEventFilter(obj, ev);
                }
            }
        }
    }
    else if (ev->type() == QEvent::MouseButtonPress)
    {
        if (this->grabbing != nullptr)
        {
            grabButtonClicked(this->grabbing);
        }
    }
    
    return false;
}
*/

/*
bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(result);
    
    if (eventType == "xcb_generic_event_t") {
        xcb_generic_event_t* xev = static_cast<xcb_generic_event_t *>(message);
        
        qDebug() << xev;
        
        if ((xev->response_type & ~0x80) == XCB_KEY_PRESS) {
              xcb_key_press_event_t* kp = (xcb_key_press_event_t*)xev;
        
              const quint32 keycode = kp->detail;
              const quint32 keymod =
                  static_cast<quint32>(kp->state & (ShiftMask | ControlMask |
                                                    Mod1Mask | Mod4Mask));
        }
    }
    
    return false;
}
*/

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    qDebug() << "native event"; 
}

