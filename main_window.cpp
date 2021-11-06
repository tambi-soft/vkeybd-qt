#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    MenuBar *menu = new MenuBar;
    setMenuBar(menu);
    
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout;
    w->setLayout(l);
    
    this->button_grab = new QPushButton("Grab Keyboard");
    connect(button_grab, &QPushButton::clicked, this, &MainWindow::grabButtonClicked);
    
    this->tabs = new MainTabs;
    
    l->addWidget(button_grab);
    l->addWidget(this->tabs);
    
    setCentralWidget(w);
    
    this->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    
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
    
    return false;
}

void MainWindow::grabButtonClicked()
{
    if (this->grabbing)
    {
        this->grabbing = false;
        releaseKeyboard();
        
        QString stylesheet = "QPushButton {"
                             "  color: black;"
                             "  background-color: white;"
                             "}";
        this->button_grab->setStyleSheet(stylesheet);
    }
    else
    {
        this->grabbing = true;
        grabKeyboard();
        
        QString stylesheet = "QPushButton {"
                             "  color: white;"
                             "  background-color: black;"
                             "}";
        this->button_grab->setStyleSheet(stylesheet);
    }
}
