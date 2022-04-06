#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QKeyEvent>
#include <QFile>
#include <QLineEdit>
#include <QSpinBox>

//#include "orgelwerk.h"
//#include "keyboard_piano.h"
#include "main_tabs.h"
#include "menubar.h"
#include "db_adapter.h"
#include "config.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(bool is_sattelite, int number_of_keyboards, QWidget *parent = nullptr);
    ~MainWindow();
    
    void saveAllParams();
    void openAllParams();

private:
    //KeyboardPiano *piano;
    //Orgelwerk *werk;
    MainTabs *tabs;
    Config *config;
    
    bool grabbing = false;
    bool ctrl_down = false;
    QPushButton *button_grab;
    
    QMap<QString, QString> key_keycode_map = {};
    
    QWidget* newKeyboardInstance(QString mode);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *);

private slots:
    void grabButtonClicked();

};
#endif // MAINWINDOW_H
