#include "main_tabs.h"

MainTabs::MainTabs(QTabWidget *parent) : QTabWidget(parent)
{
    setMovable(false);
    
    this->list_function_keys = {Qt::Key_F1, Qt::Key_F2, Qt::Key_F3, Qt::Key_F4, -1, Qt::Key_F5, Qt::Key_F6, Qt::Key_F7, Qt::Key_F8, -1, Qt::Key_F9, Qt::Key_F10, Qt::Key_F11, Qt::Key_F12};
    this->list_labels = {"F1", "F2", "F3", "F4", "spacer", "F5", "F6", "F7", "F8", "spacer", "F9", "F10", "F11", "F12"};
    
    for (int i=0; i < this->list_labels.length(); i++)
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
            addOrganTab(this->list_labels.at(i));
        }
    }
    
    // make hidden tabs invisible
    QString style = "QTabBar::tab:disabled {"
            "    width: 20px;"
            "    color: transparent;"
            "    background: transparent;"
            "}";
    setStyleSheet(style);
    
    // otherwise keys like tab and arrows are used for default behaviour
    grabKeyboard();
    
    installEventFilter(this);
}

void MainTabs::addOrganTab(QString label)
{
    Orgelwerk *o = new Orgelwerk;
    addTab(o, label);
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
            Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget());
            
            // bind esc as the panic-key
            if (event->key() == Qt::Key_Escape)
            {
                o->button_panic->animateClick();
            }
            // activate desired tab by pressing an f-key
            else if (this->list_function_keys.contains(event->key()))
            {
                for (int i=0; i < this->list_function_keys.length(); i++)
                {
                    if (this->list_function_keys.at(i) == event->key())
                    {
                        this->setCurrentIndex(i);
                    }
                }
            }
            // use the arrow-keys to operate the pitch-wheel
            else if (event->key() == Qt::Key_Left | event->key() == Qt::Key_Right | event->key() == Qt::Key_Up | event->key() == Qt::Key_Down)
            {
                o->movePitchWheel(event->key());
                return true;
            }
            // input for the virtual keyboard(s)
            else
            {
                o->keyDown(event->key());
            }
            
            return true;
        }
    }
    else if (ev->type() == QEvent::KeyRelease)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(ev);
        
        if (!event->isAutoRepeat())
        {
            Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget());
            o->keyUp(event->key());
            
            return true;
        }
    }
    
    return false;
}
