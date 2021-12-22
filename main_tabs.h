#ifndef MAINTABS_H
#define MAINTABS_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>

#include "orgelwerk.h"
#include "config.h"

class MainTabs : public QTabWidget
{
    Q_OBJECT
public:
    explicit MainTabs(Config* config, QTabWidget *parent = nullptr);
    
    bool callEventFilter(QObject *obj, QEvent *ev);
    void saveAllParams();
    void openAllParams();
    
private:
    QList<int> list_function_keys;
    QList<QString> list_labels;
    QList<Orgelwerk*> list_of_tabs;
    
    Config *config;
    
    void addOrganTab(QString label, int number_of_orgelwerks=1);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    
private slots:
    
signals:
    
};

#endif // MAINTABS_H
