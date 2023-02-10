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
    
    QAction *actionExit = new QAction(QIcon::fromTheme("application-exit"), "Exit");
    //exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    actionExit->setStatusTip("Exit Application");
    connect(actionExit, &QAction::triggered, this, &MenuBar::quitApplication);
        
    QAction *actionQuickSave = new QAction(QIcon::fromTheme("document-save"), "QuickSave Instruments");
    //saveAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
    connect(actionQuickSave, &QAction::triggered, this, &MenuBar::quickSave);
    
    QAction *actionQuickLoad = new QAction(QIcon::fromTheme("document-open"), "QuickLoad Instruments");
    connect(actionQuickLoad, &QAction::triggered, this, [this]{ emit signalLoadQuick(); });
    
    QAction *actionSave = new QAction(QIcon::fromTheme("document-save"), "Save Instruments");
    connect(actionSave, &QAction::triggered, this, [this]{ emit signalSave(); });
    
    QAction *actionLoad = new QAction(QIcon::fromTheme("document-open"), "Load Instruments");
    connect(actionLoad, &QAction::triggered, this, [this]{ emit signalLoad(); });
    
    
    
    QAction *actionSaveJackQuick = new QAction(QIcon::fromTheme("document-save"), "QuickSave JACK Connections");
    connect(actionSaveJackQuick, &QAction::triggered, this, [this]{ emit signalJackSaveQuick(); });
    
    QAction *actionLoadJackQuick = new QAction(QIcon::fromTheme("document-open"), "QuickLoad JACK Connections");
    connect(actionLoadJackQuick, &QAction::triggered, this, [this]{ emit signalJackLoadQuick(); });
    
    QAction *actionSaveJack = new QAction(QIcon::fromTheme("document-save"), "Save JACK Connections");
    connect(actionSaveJack, &QAction::triggered, this, [this]{ emit signalJackSave(); });
    
    QAction *actionLoadJack = new QAction(QIcon::fromTheme("document-open"), "Load JACK Connections");
    connect(actionLoadJack, &QAction::triggered, this, [this]{ emit signalJackLoad(); });
    
    menuFile->addAction(actionQuickSave);
    menuFile->addAction(actionQuickLoad);
    //menuFile->addSeparator();
    menuFile->addAction(actionSave);
    menuFile->addAction(actionLoad);
    menuFile->addSeparator();
    menuFile->addAction(actionSaveJackQuick);
    menuFile->addAction(actionLoadJackQuick);
    //menuFile->addSeparator();
    menuFile->addAction(actionSaveJack);
    menuFile->addAction(actionLoadJack);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);
}
void MenuBar::addToolsMenu()
{
    QMenu *menuTools = addMenu("Tools");
    
    QAction *globalKeyShift = new QAction("Global Key Shift");
    connect(globalKeyShift, &QAction::triggered, this, &MenuBar::globalKeyShift);
    
    QAction *globalResendMIDISettings = new QAction("Resend all MIDI Settings");
    connect(globalResendMIDISettings, &QAction::triggered, this, &MenuBar::globalResendMIDISettings);
    
    QAction *actionResetAll = new QAction();
    
    QAction *actionResetTab = new QAction();
    
    QAction *actionCopyTab = new QAction();
    
    QAction *actionPasteTab = new QAction();
    
    menuTools->addAction(globalKeyShift);
    menuTools->addAction(globalResendMIDISettings);
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

void MenuBar::quickSave()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "QuickSave", "Overwrite QuickSave?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        emit signalSaveQuick();
    }
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

void MenuBar::globalKeyShift()
{
    emit signalGlobalKeyShift();
}

void MenuBar::globalResendMIDISettings()
{
    emit signalResendMIDISettings();
}
