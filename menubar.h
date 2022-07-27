#ifndef MENUBAR_H
#define MENUBAR_H

#include <QObject>
#include <QWidget>
#include <QMenuBar>
#include <QAction>
#include <QApplication>

#include "enums.h"

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
    void signalSave();
    void signalOpen();
    void signalShowActionChanged(GUIElements elements, bool is_checked);
    
    void signalGlobalKeyShift();

private slots:
    void showNetworkActionChanged();
    void showKeysActionChanged();
    void showPitchActionChanged();
    void showNotesActionChanged();
    void showPCKeyboardActionChanged();
    void showPianoKeyboardActionChanged();
    
    void globalKeyShift();
};

#endif // MENUBAR_H
