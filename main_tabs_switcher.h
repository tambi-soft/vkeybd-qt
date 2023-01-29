#ifndef MAINTABSSWITCHER_H
#define MAINTABSSWITCHER_H

#include <QObject>
#include <QWidget>

#include <QGridLayout>
#include <QPushButton>
#include <QList>

#include <QDebug>

#include "config.h"

class MainTabsSwitcher : public QWidget
{
    Q_OBJECT
public:
    explicit MainTabsSwitcher(int keyboard_id = 0, Config *config = nullptr, QWidget *parent = nullptr);
    
    void pressButton(int button_id);
    QList<QString> getLabelsList();
    
private:
    int keyboard_id;
    Config *config;
    QGridLayout *grid;
    
    QList<QPushButton*> list_of_buttons;
    QList<QString> list_labels;
    
    void buttonPressed(QPushButton *button, int tab_id);
    
signals:
    void signalTabSwitched(int keyboard_id, int tab_id);
};

#endif // MAINTABSSWITCHER_H
