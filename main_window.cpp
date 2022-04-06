#include "main_window.h"

MainWindow::MainWindow(bool is_sattelite, QWidget *parent)
    : QMainWindow(parent)
{
    MenuBar *menu = new MenuBar;
    connect(menu, &MenuBar::signalSave, this, &MainWindow::saveAllParams);
    connect(menu, &MenuBar::signalOpen, this, &MainWindow::openAllParams);
    setMenuBar(menu);
    
    QWidget *main_container_widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    main_container_widget->setLayout(layout);
    setCentralWidget(main_container_widget);
    
    if (is_sattelite)
    {
        layout->addWidget(newKeyboardInstance("udp_client"));
    }
    else
    {
        layout->addWidget(newKeyboardInstance("default"));
    }
    //layout->addWidget(newKeyboardInstance("default"));
    //layout->addWidget(newKeyboardInstance("udp_client"));
    
    
}

MainWindow::~MainWindow()
{
    
}

QWidget* MainWindow::newKeyboardInstance(QString mode)
{
    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    widget->setLayout(layout);
    
    this->config = new Config;
    
    this->button_grab = new QPushButton("Grab Keyboard");
    connect(button_grab, &QPushButton::clicked, this, &MainWindow::grabButtonClicked);
    this->button_grab->setObjectName("button_grab");
    
    QLineEdit *line_udp_ip = new QLineEdit(this);
    line_udp_ip->setText("127.0.0.1");
    
    QSpinBox *spin_port = new QSpinBox;
    spin_port->setMinimum(1025);
    spin_port->setMaximum(65535);
    spin_port->setValue(20020);
    
    this->tabs = new MainTabs(this->config, mode, line_udp_ip, spin_port);
    
    layout->addWidget(button_grab);
    
    if (mode == "default")
    {
        QLabel *label_udp_listen_ip = new QLabel("IP-Address of the interface you want to listen on");
        layout->addWidget(label_udp_listen_ip);
        layout->addWidget(line_udp_ip);
        
        QLabel *label_udp_listen_port = new QLabel("Network Listen Port");
        layout->addWidget(label_udp_listen_port);
        layout->addWidget(spin_port);
        
        
    }
    else if (mode == "udp_client")
    {
        QLabel *label_udp_client_ip = new QLabel("IP-Address of remote vkeybd-qt instance:");
        layout->addWidget(label_udp_client_ip);
        
        layout->addWidget(line_udp_ip);
        
        QLabel *label_udp_client_port = new QLabel("Port:");
        layout->addWidget(label_udp_client_port);
        
        layout->addWidget(spin_port);
    }
    
    layout->addWidget(this->tabs);
    
    this->installEventFilter(this);
    //installNativeEventFilter(this);
    
    //int width = this->width();
    //resize(width, 900);
    
    //setAttribute(Qt::WA_TranslucentBackground);
    QFile css_file(":css_light");
    if (css_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        setStyleSheet(css_file.readAll());
    }
    
    return widget;
}

void MainWindow::saveAllParams()
{
    this->tabs->saveAllParams();
}
void MainWindow::openAllParams()
{
    this->tabs->openAllParams();
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
                return this->tabs->callEventFilter(obj, ev);
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
                return this->tabs->callEventFilter(obj, ev);
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
