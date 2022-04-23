#include "menubar.h"

MenuBar::MenuBar(QMenuBar *parent) : QMenuBar(parent)
{
    addFileMenu();
    addToolsMenu();
    addViewMenu();
    addHelpMenu();
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
}
void MenuBar::addToolsMenu()
{
    QMenu *menuTools = addMenu("Tools");
    
    QAction *actionResetAll = new QAction();
    
    QAction *actionResetTab = new QAction();
    
    QAction *actionCopyTab = new QAction();
    
    QAction *actionPasteTab = new QAction();
}
void MenuBar::addViewMenu()
{
    QMenu *menuView = addMenu("View");
    
    QAction *showNetwork = new QAction("Show Network Section");
    showNetwork->setCheckable(true);
    showNetwork->setChecked(true);
    
    QAction *showKeyShift = new QAction("Show Key Shift Section");
    showKeyShift->setCheckable(true);
    showKeyShift->setChecked(true);
    
    QAction *showKeys = new QAction("Show Keys Section");
    showKeys->setCheckable(true);
    showKeys->setChecked(true);
    
    QAction *showNotes = new QAction("Show Notes Section");
    showNotes->setCheckable(true);
    showNotes->setChecked(true);
    
    QAction *showPCKeyboard = new QAction("Show PC-Keyboard Section");
    showPCKeyboard->setCheckable(true);
    showPCKeyboard->setChecked(true);
    
    QAction *showPianoKeyboard = new QAction("Show Piano-Keyboard Section");
    showPianoKeyboard->setCheckable(true);
    showPianoKeyboard->setChecked(false);
    
    menuView->addAction(showNetwork);
    menuView->addAction(showKeyShift);
    menuView->addAction(showKeys);
    menuView->addAction(showNotes);
    menuView->addAction(showPCKeyboard);
    menuView->addAction(showPianoKeyboard);
}
void MenuBar::addHelpMenu()
{
    QMenu *menuHelp = addMenu("Help");
    
    //QAction *helpAction = new QAction();
}

void MenuBar::quitApplication()
{
    QApplication::quit();
}
