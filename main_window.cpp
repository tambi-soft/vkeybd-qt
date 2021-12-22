#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    MenuBar *menu = new MenuBar;
    connect(menu, &MenuBar::signalSave, this, &MainWindow::saveAllParams);
    connect(menu, &MenuBar::signalOpen, this, &MainWindow::openAllParams);
    setMenuBar(menu);
    
    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    widget->setLayout(layout);
    
    this->config = new Config;
    
    this->button_grab = new QPushButton("Grab Keyboard");
    connect(button_grab, &QPushButton::clicked, this, &MainWindow::grabButtonClicked);
    QString stylesheet = "QPushButton {"
                         "  color: white;"
                         "  background-color: darkgreen;"
                         "}";
    this->button_grab->setStyleSheet(stylesheet);
    
    this->tabs = new MainTabs(this->config);
    
    layout->addWidget(button_grab);
    layout->addWidget(this->tabs);
    
    setCentralWidget(widget);
    
    this->installEventFilter(this);
    //installNativeEventFilter(this);
    
    //int width = this->width();
    //resize(width, 900);
}

MainWindow::~MainWindow()
{
    
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
        
        QString stylesheet = "QPushButton {"
                             "  color: white;"
                             "  background-color: darkgreen;"
                             "}";
        this->button_grab->setStyleSheet(stylesheet);
        this->button_grab->setText("Grab Keyboard");
    }
    else
    {
        this->grabbing = true;
        grabKeyboard();
        grabMouse();
        
        QString stylesheet = "QPushButton {"
                             "  color: white;"
                             "  background-color: darkblue;"
                             "}";
        this->button_grab->setStyleSheet(stylesheet);
        this->button_grab->setText("Grabbing Keyboard. Hit Ctrl+Shift or Click to stop.");
    }
}
