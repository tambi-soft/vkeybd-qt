#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //this->piano = new KeyboardPiano;
    this->werk = new Orgelwerk;
    
    /*
    setCentralWidget(this->piano);
    resize(this->piano->size);
    */
    
    setCentralWidget(this->werk);
    //resize(this->werk->size);
    
    this->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    
}

/*
bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::KeyPress)
    {
        QString keycode = "cis1";
        
        QKeyEvent *key = static_cast<QKeyEvent*>(ev);
        if (!key->isAutoRepeat())
        {
            qDebug() << key;
            
            if (key->key() == Qt::Key_A)
            {
                keycode = "a1";
            }
            else if (key->key() == Qt::Key_B)
            {
                keycode = "b1";
            }
            this->piano->keyPressed(keycode);
        }
    }
    else if (ev->type() == QEvent::KeyRelease)
    {
        QString keycode = "cis1";
        
        QKeyEvent *key = static_cast<QKeyEvent*>(ev);
        if (!key->isAutoRepeat())
        {
            qDebug() << key;
            if (key->key() == Qt::Key_A)
            {
                keycode = "a1";
            }
            else if (key->key() == Qt::Key_B)
            {
                keycode = "b1";
            }
            
            this->piano->keyReleased(keycode);
        }
    }
    return QObject::eventFilter(obj, ev);
}
*/
