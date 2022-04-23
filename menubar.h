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
    
    QAction *showNetwork;
    QAction *showKeyShift;
    QAction *showKeys;
    QAction *showNotes;
    QAction *showPCKeyboard;
    QAction *showPianoKeyboard;
    
signals:
    void signalSave();
    void signalOpen();
    void signalShowActionChanged(QString name, bool is_checked);

private slots:
    void showNetworkActionChanged();
    void showKeyShiftActionChanged();
    void showKeysActionChanged();
    void showNotesActionChanged();
    void showPCKeyboardActionChanged();
    void showPianoKeyboardActionChanged();
    
};

#endif // MENUBAR_H
