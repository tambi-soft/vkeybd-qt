#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    MenuBar *menu = new MenuBar;
    setMenuBar(menu);
    
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout;
    w->setLayout(l);
    
    //this->piano = new KeyboardPiano;
    //this->werk = new Orgelwerk;
    
    this->button_grab = new QPushButton("Grab Keyboard");
    connect(button_grab, &QPushButton::clicked, this, &MainWindow::grabButtonClicked);
    
    this->tabs = new MainTabs;
    
    l->addWidget(button_grab);
    l->addWidget(this->tabs);
    
    /*
    setCentralWidget(this->piano);
    resize(this->piano->size);
    */
    
    //setCentralWidget(this->werk);
    //resize(this->werk->size);
    //setCentralWidget(this->tabs);
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
    //Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget());
    
    if (this->grabbing)
    {
        this->grabbing = false;
        releaseKeyboard();
        
        //o->button_grab->setDown(false);
        QString stylesheet = "QPushButton {"
                             "  color: black;"
                             "  background-color: white;"
                             "}";
        //o->button_grab->setStyleSheet(stylesheet);
        this->button_grab->setStyleSheet(stylesheet);
    }
    else
    {
        this->grabbing = true;
        grabKeyboard();
        
        //o->button_grab->setDown(true);
        QString stylesheet = "QPushButton {"
                             "  color: white;"
                             "  background-color: black;"
                             "}";
        //o->button_grab->setStyleSheet(stylesheet);
        this->button_grab->setStyleSheet(stylesheet);
    }
}
