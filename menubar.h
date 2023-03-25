#ifndef MENUBAR_H
#define MENUBAR_H

#include <QObject>
#include <QWidget>
#include <QMenuBar>
#include <QAction>
#include <QApplication>
#include <QMessageBox>

#include <QDebug>

#include "enums_structs.h"

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
    
    QAction *showNetwork;
    QAction *showKeyShift;
    QAction *showKeys;
    QAction *showPitch;
    QAction *showNotes;
    QAction *showPCKeyboard;
    QAction *showPianoKeyboard;
    
signals:
    void signalSaveQuick();
    void signalLoadQuick();
    void signalSave();
    void signalLoad();
    
    void signalMIDISaveQuick();
    void signalMIDILoadQuick();
    void signalMIDISave();
    void signalMIDILoad();
    
    void signalShowActionChanged(GUIElements elements, bool is_checked);
    
    void signalGlobalKeyShift();
    void signalResendMIDISettings();
    
    void signalHelpAbout();

private slots:
    void quickSave();
    
    void showNetworkActionChanged();
    void showKeysActionChanged();
    void showPitchActionChanged();
    void showNotesActionChanged();
    void showPCKeyboardActionChanged();
    void showPianoKeyboardActionChanged();
    
    void globalKeyShift();
    void globalResendMIDISettings();
};

#endif // MENUBAR_H
