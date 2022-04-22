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
    
    if (output_system == "network")
    {
        layout->addWidget(newKeyboardInstance(0, output_system));
    }
    else
    {
        MenuBar *menu = new MenuBar;
        connect(menu, &MenuBar::signalSave, this, &MainWindow::saveAllParams);
        connect(menu, &MenuBar::signalOpen, this, &MainWindow::openAllParams);
        setMenuBar(menu);
        
        for (int i=0; i < number_of_keyboards; i++)
        {
            layout->addWidget(newKeyboardInstance(i, output_system));
        }
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
    
    QPushButton *button_grab = new QPushButton("Grab Keyboard");
    connect(button_grab, &QPushButton::clicked, this, [this, button_grab]{ grabButtonClicked(button_grab); });
    button_grab->setObjectName("button_grab");
    this->list_of_button_grabs.append(button_grab);
    
    this->line_udp_ip = new QLineEdit(this);
    this->line_udp_ip->setText("127.0.0.1");
    this->line_udp_ip->setToolTip("For remote-controlling: IP-Address of the interface you want to listen on (the IP-address of this machine).");
    this->line_udp_ip->setObjectName("network_select");
    
    this->spin_port = new QSpinBox;
    this->spin_port->setMinimum(1025);
    this->spin_port->setMaximum(65535);
    this->spin_port->setValue(20020);
    this->spin_port->setToolTip("Network Listen Port");
    this->spin_port->setObjectName("network_select");
    
    MainTabs *tabs = new MainTabs(id, this->config, mode, this->line_udp_ip, this->spin_port);
    this->list_of_maintabs.append(tabs);
    
    grid->addWidget(button_grab, 0, 0, 1, 3);
    
    if (mode != "network")
    {
        QPushButton *button_network_help = new QPushButton;
        button_network_help->setIcon(QIcon::fromTheme("dialog-question"));
        button_network_help->setToolTip("help");
        connect(button_network_help, &QPushButton::clicked, this, []{ new HelpMessage(":help_tntware"); });
        
        grid->addWidget(line_udp_ip, 1, 0);
        grid->addWidget(spin_port, 1, 1);
        grid->addWidget(button_network_help, 1, 2);
    }
    else if (mode == "network")
    {
        QLabel *label_udp_client_ip = new QLabel("IP-Address of remote vkeybd-qt instance:");
        grid->addWidget(label_udp_client_ip, 0, 0, 1, 3);
        
        grid->addWidget(line_udp_ip, 1, 0, 1, 3);
        
        QLabel *label_udp_client_port = new QLabel("Port:");
        grid->addWidget(label_udp_client_port, 2, 0, 1, 3);
        
        grid->addWidget(spin_port, 2, 0, 1, 3);
    }
    
    grid->addWidget(tabs, 3, 0, 1, 3);
    
    this->installEventFilter(this);
    //installNativeEventFilter(this);
    
    //int width = this->width();
    //resize(width, 900);
    
    return widget;
}

void MainWindow::saveAllParams()
{
    QString network_ip = this->line_udp_ip->text();
    int network_port = this->spin_port->value();
    
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

bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *)
{
    return false;
}

void MainWindow::grabButtonClicked(QPushButton *button_grab)
{
    if (this->grabbing != nullptr)
    {
        releaseKeyboard();
        // to avoid to mess around with the os like blocking taskbar-items, we need to grab the mouse aswell
        releaseMouse();
        
        this->grabbing->setDown(false);
        this->grabbing->setText("Grab Keyboard");
        
        this->grabbing = nullptr;
    }
    else
    {
        this->grabbing = button_grab;
        this->grabbing_last = button_grab;
        
        grabKeyboard();
        grabMouse();
        
        this->grabbing->setDown(true);
        this->grabbing->setText("Grabbing Keyboard. Hit Ctrl+Shift or Click to stop.");
    }
}
