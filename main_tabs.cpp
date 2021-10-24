#include "main_tabs.h"

MainTabs::MainTabs(QTabWidget *parent) : QTabWidget(parent)
{
    setMovable(false);
    
    this->list_keys = {Qt::Key_F1, Qt::Key_F2, Qt::Key_F3, Qt::Key_F4, -1, Qt::Key_F5, Qt::Key_F6, Qt::Key_F7, Qt::Key_F8, -1, Qt::Key_F9, Qt::Key_F10, Qt::Key_F11, Qt::Key_F12};
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
}

void MainTabs::addOrganTab(QString label)
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(layout);
    
    Orgelwerk *werk1 = new Orgelwerk;
    Orgelwerk *werk2 = new Orgelwerk;
    
    layout->addWidget(werk1);
    layout->addWidget(werk2);
    
    addTab(widget, label);
}

void MainTabs::keyPressEvent(QKeyEvent *ev)
{
    for (int i=0; i < this->list_keys.length(); i++)
    {
        if (this->list_keys.at(i) == ev->key())
        {
            this->setCurrentIndex(i);
        }
    }
}
