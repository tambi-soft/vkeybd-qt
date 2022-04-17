#include "menubar.h"

MenuBar::MenuBar(QMenuBar *parent) : QMenuBar(parent)
{
    addFileMenu();
}

void MenuBar::addFileMenu()
{
    QMenu *menuFile = addMenu("File");
    
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "Exit");
    //exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit Application");
    connect(exitAction, &QAction::triggered, this, &MenuBar::quitApplication);
        
    QAction *saveAction = new QAction(QIcon::fromTheme("application-save"), "Save Settings");
    //saveAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
    connect(saveAction, &QAction::triggered, this, [this]{ emit signalSave(); });
    
    QAction *openAction = new QAction(QIcon::fromTheme("application-open"), "Open Settings");
    connect(openAction, &QAction::triggered, this, [this]{ emit signalOpen(); });
    
    menuFile->addAction(saveAction);
    menuFile->addAction(openAction);
    menuFile->addAction(exitAction);
    
    
    
    QMenu *menuTools = addMenu("Tools");
    
    QAction *actionResetAll = new QAction();
    
    QAction *actionResetTab = new QAction();
    
    QAction *actionCopyTab = new QAction();
    
    QAction *actionPasteTab = new QAction();
    
    QMenu *menuView = addMenu("View");
    
    QMenu *menuHelp = addMenu("Help");
    
    
    //QAction *helpAction = new QAction();
    
}

void MenuBar::quitApplication()
{
    QApplication::quit();
}
