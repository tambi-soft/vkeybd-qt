#ifndef MAINTABS_H
#define MAINTABS_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>

#include "orgelwerk.h"

class MainTabs : public QTabWidget
{
    Q_OBJECT
public:
    explicit MainTabs(QTabWidget *parent = nullptr);
    
private:
    QList<int> list_keys;
    QList<QString> list_labels;
    
    void addOrganTab(QString label);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    
private slots:
    void keyPressEvent(QKeyEvent* ev);
    
signals:
    
};

#endif // MAINTABS_H
