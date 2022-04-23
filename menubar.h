#ifndef MENUBAR_H
#define MENUBAR_H

#include <QObject>
#include <QWidget>
#include <QMenuBar>
#include <QAction>
#include <QApplication>

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QMenuBar *parent = nullptr);
    
private:
    void addFileMenu();
    void addToolsMenu();
    void addViewMenu();
    void addHelpMenu();
    
    void quitApplication();
    
signals:
    void signalSave();
    void signalOpen();
    
};

#endif // MENUBAR_H
