#include "main_window.h"

MainWindow::MainWindow(QString output_system, int number_of_keyboards, QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *main_container_widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    main_container_widget->setLayout(layout);
    setCentralWidget(main_container_widget);
    
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
    
    this->button_grab = new QPushButton("Grab Keyboard");
    connect(button_grab, &QPushButton::clicked, this, &MainWindow::grabButtonClicked);
    this->button_grab->setObjectName("button_grab");
    
    QLineEdit *line_udp_ip = new QLineEdit(this);
    line_udp_ip->setText("127.0.0.1");
    line_udp_ip->setToolTip("For remote-controlling: IP-Address of the interface you want to listen on (the IP-address of this machine).");
    
    QSpinBox *spin_port = new QSpinBox;
    spin_port->setMinimum(1025);
    spin_port->setMaximum(65535);
    spin_port->setValue(20020);
    spin_port->setToolTip("Network Listen Port");
    
    MainTabs *tabs = new MainTabs(id, this->config, mode, line_udp_ip, spin_port);
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
    
    QFile css_file(":css_light");
    if (css_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        setStyleSheet(css_file.readAll());
    }
    
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
    //this->tabs->openAllParams();
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
                    grabButtonClicked();
                    
                    return true;
                }
            }
            else
            {
                //return this->tabs->callEventFilter(obj, ev);
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
            }
        }
    }
    else if (ev->type() == QEvent::MouseButtonPress)
    {
        if (this->grabbing)
        {
            grabButtonClicked();
        }
    }
    
    return false;
}

bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *)
{
    return false;
}

void MainWindow::grabButtonClicked()
{
    if (this->grabbing)
    {
        this->grabbing = false;
        releaseKeyboard();
        // to avoid to mess around with the os like blocking taskbar-items, we need to grab the mouse aswell
        releaseMouse();
        
        this->button_grab->setDown(false);
        this->button_grab->setText("Grab Keyboard");
    }
    else
    {
        this->grabbing = true;
        grabKeyboard();
        grabMouse();
        
        this->button_grab->setDown(true);
        this->button_grab->setText("Grabbing Keyboard. Hit Ctrl+Shift or Click to stop.");
    }
}
