#include "main_tabs.h"

MainTabs::MainTabs(QTabWidget *parent) : QTabWidget(parent)
{
    addOrganTab("F1");
    addOrganTab("F2");
    addOrganTab("F3");
    addOrganTab("F4");
}

void MainTabs::addOrganTab(QString label)
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    widget->setLayout(layout);
    
    Orgelwerk *werk1 = new Orgelwerk;
    Orgelwerk *werk2 = new Orgelwerk;
    
    layout->addWidget(werk1);
    layout->addWidget(werk2);
    
    addTab(widget, label);
}
