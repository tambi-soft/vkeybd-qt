#ifndef MAIN_STACKED_WIDGET_H
#define MAIN_STACKED_WIDGET_H

#include <QObject>
#include <QWidget>

#include <QGridLayout>
#include <QPushButton>
#include <QSpacerItem>

#include <QDebug>

class StackedWidgetSwitches : public QWidget
{
    Q_OBJECT
public:
    explicit StackedWidgetSwitches(QWidget *parent = nullptr);
    
private:
    QList<int> list_function_keys;
    QList<QString> list_labels;
    
signals:
    
};




class MainStackedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainStackedWidget(QWidget *parent = nullptr);
    
signals:
    
};

#endif // MAIN_STACKED_WIDGET_H
