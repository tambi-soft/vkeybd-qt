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
#include <QComboBox>
#include <QIcon>
#include <QAbstractNativeEventFilter>
#include <QAbstractEventDispatcher>
#include <QStyle>
#include <QStyledItemDelegate>

#include "main_tabs.h"
#include "menubar.h"
#include "config.h"
#include "help_message.h"
#include "help_about_widget.h"

#include "input_keyboard_qt.h"
#include "input_keyboard_raw.h"
#include "input_keyboard_xcb.h"
#include "combo_keyboard_select.h"

#include "enums.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(OutputSystem output, int number_of_keyboards, QWidget *parent = nullptr);
    ~MainWindow();

private:
    QList<MainTabs*> list_of_maintabs;
    Config *config;
    int number_of_keyboards;
    
    QList<QLineEdit*> list_of_line_udp_ips;
    QList<QSpinBox*> list_of_spin_ports;
    QList<QPushButton*> list_of_network_help_buttons;
    
    QMap<QString, QString> key_keycode_map = {};
    
    QWidget* newKeyboardInstance(int id, OutputSystem output);
    
    //bool input_kbd_qt_native = true;
    //bool input_kbd_qt_default = false;
    int keyboard_selection;
    
    InputKeyboardQt *inputQt;
    InputKeyboardXCB *inputXCB;

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private slots:
    void saveAllParams();
    void openAllParams();
    void restoreParams(int maintab, QString tab, QMap<QString, QVariant> data);
    void restoreGeneral(int maintab, QMap<QString,QVariant> data);
    
    void showActionChanged(GUIElements elements, bool is_checked);
    
    void keyboardSelectionChanged(int selection, bool locked);
    
    void rawKeyPressed(int key);
    void rawKeyReleased(int key);
    void changeTab(int id);
    //void MIDISignal(MIDISignalTypes type);

};
#endif // MAINWINDOW_H
