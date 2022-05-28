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
    
    this->showNetwork = new QAction("Show Network Section");
    this->showNetwork->setCheckable(true);
    this->showNetwork->setChecked(false);
    connect(this->showNetwork, &QAction::changed, this, &MenuBar::showNetworkActionChanged);
    
    this->showKeys = new QAction("Show Keys Section");
    this->showKeys->setCheckable(true);
    this->showKeys->setChecked(true);
    connect(this->showKeys, &QAction::changed, this, &MenuBar::showKeysActionChanged);
    
    this->showPitch = new QAction("Show Pitchwheel and Volume Section");
    this->showPitch->setCheckable(true);
    this->showPitch->setChecked(true);
    connect(this->showPitch, &QAction::changed, this, &MenuBar::showPitchActionChanged);
    
    this->showNotes = new QAction("Show Notes Section");
    this->showNotes->setCheckable(true);
    this->showNotes->setChecked(true);
    connect(this->showNotes, &QAction::changed, this, &MenuBar::showNotesActionChanged);
    
    this->showPCKeyboard = new QAction("Show PC-Keyboard Section");
    this->showPCKeyboard->setCheckable(true);
    this->showPCKeyboard->setChecked(true);
    connect(this->showPCKeyboard, &QAction::changed, this, &MenuBar::showPCKeyboardActionChanged);
    
    this->showPianoKeyboard = new QAction("Show Piano-Keyboard Section");
    this->showPianoKeyboard->setCheckable(true);
    this->showPianoKeyboard->setChecked(false);
    connect(this->showPianoKeyboard, &QAction::changed, this, &MenuBar::showPianoKeyboardActionChanged);
    
    menuView->addAction(showNetwork);
    menuView->addAction(showKeys);
    menuView->addAction(showPitch);
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

void MenuBar::showNetworkActionChanged()
{
    emit signalShowActionChanged(GUIElements::Network, this->showNetwork->isChecked());
}
void MenuBar::showKeysActionChanged()
{
    emit signalShowActionChanged(GUIElements::MIDIKeys, this->showKeys->isChecked());
}
void MenuBar::showPitchActionChanged()
{
    emit signalShowActionChanged(GUIElements::PitchAndVolume, this->showPitch->isChecked());
}
void MenuBar::showNotesActionChanged()
{
    emit signalShowActionChanged(GUIElements::Notes, this->showNotes->isChecked());
}
void MenuBar::showPCKeyboardActionChanged()
{
    emit signalShowActionChanged(GUIElements::KeyboardPC, this->showPCKeyboard->isChecked());
}
void MenuBar::showPianoKeyboardActionChanged()
{
    emit signalShowActionChanged(GUIElements::KeyboardPiano, this->showPianoKeyboard->isChecked());
}
