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
    Orgelwerk *werk = new Orgelwerk;
    
    addTab(werk, label);
}
