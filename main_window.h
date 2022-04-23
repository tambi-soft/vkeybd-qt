#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QGridLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QFile>
#include <QLineEdit>
#include <QSpinBox>

//#include "orgelwerk.h"
//#include "keyboard_piano.h"
#include "main_tabs.h"
#include "menubar.h"
#include "config.h"
#include "help_message.h"
#include "help_about_widget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString output_system, int number_of_keyboards, QWidget *parent = nullptr);
    ~MainWindow();
    
    void saveAllParams();
    void openAllParams();

private:
    //KeyboardPiano *piano;
    //Orgelwerk *werk;
    //MainTabs *tabs;
    QList<MainTabs*> list_of_maintabs;
    Config *config;
    
    QList<QLineEdit*> list_of_line_udp_ips;
    QList<QSpinBox*> list_of_spin_ports;
    
    //QPushButton *button_grab;
    QList<QPushButton*> list_of_button_grabs;
    QPushButton *grabbing = nullptr;
    QPushButton *grabbing_last = nullptr;
    bool ctrl_down = false;
    
    QMap<QString, QString> key_keycode_map = {};
    
    QWidget* newKeyboardInstance(int id, QString mode);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *);

private slots:
    void grabButtonClicked(QPushButton *button_grab);
    void restoreParams(int maintab, QString tab, QMap<QString, QVariant> data);
    void restoreGeneral(int maintab, QMap<QString,QVariant> data);

};
#endif // MAINWINDOW_H
