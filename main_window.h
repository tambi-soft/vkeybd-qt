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
#include <QFrame>
#include <QFileDialog>

#include "main_tabs.h"
#include "menubar.h"
#include "config.h"
#include "help_message.h"
#include "help_about_widget.h"

#include "input_keyboard_qt.h"
#include "input_keyboard_raw.h"
#include "input_keyboard_xcb.h"
#include "combo_keyboard_select.h"

#include "midi_key_shift_widget.h"
#include "midi_key_shift_global.h"

#include "main_tabs_switcher.h"

#include "jack_save_load.h"

#include "enums_structs.h"

#include "help_about_widget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(OutputSystem output, int number_of_keyboards, QWidget *parent = nullptr);
    ~MainWindow();

private:
    QList<MainTabsSwitcher*> list_of_maintab_switchers;
    QList<MainTabs*> list_of_maintabs;
    Config *config;
    int number_of_keyboards;
    
    QList<QLineEdit*> list_of_line_udp_ips;
    QList<QSpinBox*> list_of_spin_ports;
    QList<QPushButton*> list_of_network_help_buttons;
    
    QMap<QString, QString> key_keycode_map = {};
    
    MainTabsSwitcher *switcher;
    void changeCurrentTab(int keyboard_id, int tab_id);
    void currentTabChanged(int keyboard_id, int tab_id);
    void tabsCheckChanged(QList<int> list_of_tab_ids);
    
    QWidget* newKeyboardInstance(int id, OutputSystem output);
    
    //bool input_kbd_qt_native = true;
    //bool input_kbd_qt_default = false;
    int keyboard_selection;
    
    InputKeyboardQt *inputQt = new InputKeyboardQt;
    InputKeyboardXCB *inputXCB = nullptr;
    
    MIDIKeyShiftGlobal *global_key_shift_widget = new MIDIKeyShiftGlobal;

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private slots:
    void saveParamsQuick();
    void loadParamsQuick();
    void saveParams();
    void loadParams();
    //void openAllParams();
    void restoreParams(int maintab, QString tab, QMap<QString, QVariant> data);
    void restoreGeneral(int maintab, QMap<QString,QVariant> data);
    
    void saveJACKQuick();
    void loadJACKQuick();
    void saveJACK();
    void loadJACK();
    
    void showActionChanged(GUIElements elements, bool is_checked);
    
    void keyboardSelectionChanged(int selection, bool locked);
    
    void rawKeyPressed(int key);
    void rawKeyReleased(int key);
    //void MIDISignal(MIDISignalTypes type);
    
    void showGlobalKeyShift();
    void globalKeyShiftValueChanged(int value, bool is_relative);
    
    void globalResendMIDISettings();
    
    void helpAbout();
};
#endif // MAINWINDOW_H
