#ifndef MAINTABS_H
#define MAINTABS_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>

#include "orgelwerk.h"

class MainTabs : public QTabWidget
{
    Q_OBJECT
public:
    explicit MainTabs(QTabWidget *parent = nullptr);
    
private:
    void addOrganTab(QString label);
    
signals:
    
};

#endif // MAINTABS_H
