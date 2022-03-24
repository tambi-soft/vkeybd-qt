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
    
    this->stack_widget_switches = new StackedWidgetSwitches;
    
    layout->addWidget(button_grab);
    //layout->addWidget(this->stack_widget_switches);
    layout->addWidget(this->tabs);
    
    setCentralWidget(widget);
    
    this->installEventFilter(this);
    //installNativeEventFilter(this);
    
    //int width = this->width();
    //resize(width, 900);
    
    QString style = "QWidget {"
                    "    /*color: #d4ce99;*/"
                    "    color: #e7e7e7;"
                    "    background: #393939;"
                    "    /*padding: 1px;*/"
                    "    /*margin: 1px;*/"
                    "}"
                    "QTabWidget:pane {"
                    "    border: 1px solid #94969a;"
                    "    border-top: 0px;"
                    "}"
                    "QTabBar::tab {"
                    "    font-size: 8pt;"
                    "    border: 1px solid #94969a;"
                    "}"
                    "QTabBar::tab:enabled {"
                    "    width: 30px;"
                    "    padding: 1px;"
                    "}"
                    "QTabBar::tab:selected {"
                    "    background: #393939;"
                    "    border-bottom-color: #393939;"
                    "}"
                    "QTabBar::tab:!selected {"
                    "    background: #2d2d2d;"
                    "}"
                    "QTabBar::tab:disabled {"
                    "    width: 3px;"
                    "    color: transparent;"
                    "    background: #393939;"
                    "}"
                    "QGraphicsView {"
                    "    background: #eeeeee;"
                    "}"
                    "QGroupBox {"
                    "    border: 1px solid #956d3d;"
                    "    /*padding: 5px;*/"
                    "    margin-top: 3ex;"
                    "}"
                    "QGroupBox:title {"
                    "    subcontrol-origin: margin;"
                    "    subcontrol-position: top left;"
                    "}"
                    "QCheckBox:indicator:unchecked {"
                    "    "
                    "}"
                    "QCheckBox:indicator:checked {"
                    "    color: #42b2c0;"
                    "    background-color: #cf6f2e;"
                    "}";
    setStyleSheet(style);
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
