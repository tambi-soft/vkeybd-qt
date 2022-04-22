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
        
    QAction *quickSaveAction = new QAction(QIcon::fromTheme("application-save"), "Quick Save");
    //saveAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
    connect(quickSaveAction, &QAction::triggered, this, [this]{ emit signalSave(); });
    
    QAction *quickLoadAction = new QAction(QIcon::fromTheme("application-open"), "Quick Load");
    connect(quickLoadAction, &QAction::triggered, this, [this]{ emit signalOpen(); });
    
    menuFile->addAction(quickSaveAction);
    menuFile->addAction(quickLoadAction);
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
