#ifndef MAINSTACKEDWIDEGT_H
#define MAINSTACKEDWIDEGT_H

#include <QObject>
#include <QWidget>
#include <QStackedWidget>

#include "orgelwerk.h"
#include "config.h"

class MainStackedWidegt : public QWidget
{
    Q_OBJECT
public:
    explicit MainStackedWidegt(QWidget *parent = nullptr);
    
signals:
    
};

#endif // MAINSTACKEDWIDEGT_H
