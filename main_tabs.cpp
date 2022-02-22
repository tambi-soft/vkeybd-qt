#include "main_tabs.h"

MainTabs::MainTabs(Config *config, QTabWidget *parent) : QTabWidget(parent)
{
    this->config = config;
    
    setMovable(false);
    
    this->list_function_keys = {Qt::Key_F1, Qt::Key_F2, Qt::Key_F3, Qt::Key_F4, -1, Qt::Key_F5, Qt::Key_F6, Qt::Key_F7, Qt::Key_F8, -1, Qt::Key_F9, Qt::Key_F10, Qt::Key_F11, Qt::Key_F12};
    this->list_labels = {"F1", "F2", "F3", "F4", "spacer", "F5", "F6", "F7", "F8", "spacer", "F9", "F10", "F11", "F12"};
    
    int number_of_tabs = this->list_labels.length();
    //number_of_tabs = 4;
    for (int i=0; i < number_of_tabs; i++)
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
            addOrganTab(this->list_labels.at(i), 1);
        }
    }
    
    // make hidden tabs invisible
    QString style = "QTabBar::tab:disabled {"
            "    width: 20px;"
            "    color: transparent;"
            "    background: transparent;"
            "}";
    setStyleSheet(style);
    
    installEventFilter(this);
}

void MainTabs::addOrganTab(QString label, int number_of_orgelwerks)
{
    /*
    Orgelwerk *o = new Orgelwerk(label);
    addTab(o, label);
    */
    
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    widget->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    
    QString midi_port_label = label;
    for (int i=0; i < number_of_orgelwerks; i++)
    {
        if (number_of_orgelwerks > 1)
        {
            midi_port_label = label + "-" + QString::number(i);
        }
        
        Orgelwerk *o = new Orgelwerk(midi_port_label);
        connect(o, &Orgelwerk::eventFiltered, this, &MainTabs::eventFilter);
        
        layout->addWidget(o);
        
        this->list_of_tabs.append(o);
    }
    
    addTab(widget, label);
}

void MainTabs::saveAllParams()
{
    for (int i=0; i < count(); i++)
    {
        QString label = tabText(i);
        if (! label.isEmpty())
        {
            Orgelwerk *o = static_cast<Orgelwerk*>(widget(i)->layout()->itemAt(0)->widget());
            
            QList<QMap<QString,QVariant>> channels = o->listOfChannels(false);
            this->config->saveChannelSettings(label, channels);
        }
    }
}
void MainTabs::openAllParams()
{
    for (int i=0; i < count(); i++)
    //for (int i=0; i < 1; i++)
    {
        QString label = tabText(i);
        if (! label.isEmpty())
        {
            Orgelwerk *o = static_cast<Orgelwerk*>(widget(i)->layout()->itemAt(0)->widget());
            
            QList<QMap<QString,QVariant>> channels = this->config->loadChannelSettings(label);
            o->setListOfChannels(channels);
        }
    }
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
            //Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget());
            Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget()->layout()->itemAt(0)->widget());
            
            if (event->key() == Qt::Key_Escape)
            {
                //o->button_panic->animateClick();
                for (int i=0; i < this->list_of_tabs.length(); i++)
                {
                    this->list_of_tabs.at(i)->button_panic->animateClick();
                }
                
                return true;
            }
            else if (event->key() == Qt::Key_Delete)
            {
                //o->button_stop_all->animateClick();
                for (int i=0; i < this->list_of_tabs.length(); i++)
                {
                    this->list_of_tabs.at(i)->button_stop_all->animateClick();
                }
            }
            else if (event->key() == Qt::Key_Menu)
            {
                o->button_channels_dialog->animateClick();
            }
            else if (event->key() == Qt::Key_Insert)
            {
                o->button_resend_midi->animateClick();
            }
            else if (event->key() == Qt::Key_Space)
            {
                o->keySustain(true);
                o->keyDown(Qt::Key_Space);
                return true;
            }
            else if (event->key() == Qt::Key_Alt)
            {
                o->keySostenuto(true);
                o->keyDown(Qt::Key_Alt);
                return true;
            }
            else if (event->key() == Qt::Key_Super_L || event->key() == Qt::Key_Super_R)
            {
                o->keySoft(true);
                o->keyDown(Qt::Key_Super_L);
                o->keyDown(Qt::Key_Super_R);
                return true;
            }
            // activate desired tab by pressing an f-key
            else if (this->list_function_keys.contains(event->key()))
            {
                for (int i=0; i < this->list_function_keys.length(); i++)
                {
                    if (this->list_function_keys.at(i) == event->key())
                    {
                        // send "all keys of" on old tab
                        //o->panicKeyPressed();
                        
                        // activate new tab
                        this->setCurrentIndex(i);
                        
                        // current widget changed, need to update
                        //o = static_cast<Orgelwerk*>(currentWidget()->layout()->itemAt(0)->widget());
                        //o->resendMIDIControls();
                        
                        return true;
                    }
                }
            }
            // use the arrow-keys to operate the pitch-wheel
            else if (event->key() == Qt::Key_Left | event->key() == Qt::Key_Right | event->key() == Qt::Key_Up | event->key() == Qt::Key_Down)
            //else if (event->key() == Qt::Key_Up | event->key() == Qt::Key_Down)
            {
                //o->movePitchWheel(event->key());
                o->pitch->pitchKeyPressed(event->key());
                return true;
            }
            // input for the virtual keyboard(s)
            else
            {
                o->keyDown(event->key());
                return true;
            }
        }
    }
    else if (ev->type() == QEvent::KeyRelease)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(ev);
        
        if (!event->isAutoRepeat())
        {
            //Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget());
            Orgelwerk *o = static_cast<Orgelwerk*>(currentWidget()->layout()->itemAt(0)->widget());
            
            if (event->key() == Qt::Key_Space)
            {
                o->keySustain(false);
                o->keyUp(Qt::Key_Space);
                return true;
            }
            else if (event->key() == Qt::Key_Alt)
            {
                o->keySostenuto(false);
                o->keyUp(Qt::Key_Alt);
                return true;
            }
            else if (event->key() == Qt::Key_Super_L || event->key() == Qt::Key_Super_R)
            {
                o->keySoft(false);
                o->keyUp(Qt::Key_Super_L);
                o->keyUp(Qt::Key_Super_R);
                return true;
            }
            else if (event->key() == Qt::Key_Left | event->key() == Qt::Key_Right | event->key() == Qt::Key_Up | event->key() == Qt::Key_Down)
            {
                o->pitch->pitchKeyReleased();
                return true;
            }
            else
            {
                // this function should be kept just in case "areKeysPressed" would return "false" falsely on current tab ...
                //o->keyUp(event->key());
                // we want to have a smooth way of switching between tabs (=presets) during playing
                
                for (int i=0; i < this->list_of_tabs.length(); i++)
                {
                    //if (this->list_of_tabs.at(i)->areKeysPressed())
                    //{
                        this->list_of_tabs.at(i)->keyUp(event->key());
                    //}
                }
                
                return true;
            }
        }
    }
    
    return false;
}
