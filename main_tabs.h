#ifndef MAINTABS_H
#define MAINTABS_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QSettings>

#include <QUdpSocket>
#include <QCoreApplication>

#include "orgelwerk.h"
#include "config.h"
#include "enums_structs.h"

#include "input_keyboard_select.h"
//#include "combo_keyboard_select.h"

#include "enums_structs.h"

class MainTabs : public QTabWidget
{
    Q_OBJECT
public:
    explicit MainTabs(QList<QString> labels, int id, Config* config, OutputSystem output, InputKeyboardSelect *input_keyboard_select, QLineEdit *line_udp_ip, QSpinBox *spin_port, QWidget *parent = nullptr);
    
    //bool callEventFilter(QObject *obj, QEvent *ev);
    void saveParams(QSettings *settings);
    void restoreParams(QString tab, QMap<QString,QVariant> data);
    
    void showHideGUIElements(GUIElements elements, bool show);
    
    void rawKeyPressed(int keycode);
    void rawKeyReleased(int keycode);
    void MIDISignal(MIDISignalTypes type);
    void saveMIDIConnections();
    void loadMIDIConnections();
    
    void globalKeyShiftChanged(int value, bool is_relative);
    void globalResendMIDISettings();
    
    void listOfCheckTabsChanged(QList<int> list_of_tab_ids);
    
private:
    //QList<int> list_function_keys;
    InterfaceAudio *interface_audio;
    
    QList<int> list_function_keys_raw;
    //QList<int> list_function_keys_native;
    QList<QString> list_labels;
    QList<Orgelwerk*> list_of_tabs;
    QMap<QString,Orgelwerk*> map_of_tabs;
    
    // we want to allow multiple tabs to be checked at once. they all shall sound simultaniously.
    QList<int> list_of_checked_tabs;
    
    int id;
    Config *config;
    //InputKeyboardSelect *input_keyboard_select;
    //QPushButton *button_lock;
    QLineEdit *line_udp_ip;
    QSpinBox *spin_port;
    
    QUdpSocket *socket;
    void rebindSocketIP(QString ip);
    void rebindSocket(int value);
    bool send_udp = false;
    void sendUDPMessage(QString message);
    void receiveUDPMessage();
    
    void initializeTabs(OutputSystem output);
    void addOrganTab(OutputSystem output, int tab_id, QString label);
    
    void multiplexMIDIEventToTabs(int tab_id, int midicode, MIDIMode mode);
    
    bool is_space_pressed = false;

protected:
    //bool eventFilter(QObject *obj, QEvent *ev) override;
    
private slots:
    //void keyboardSelectionChanged(QString text);
    void deviceNotAvailable(QString message);
    //void toggleKeyboardLock();
    
signals:
    void useInputKbdQtNativeSignal();
    void useInputKbdQtDefaultSignal();
    
    void signalKeyShiftChanged(int value, bool is_relative);
    
};

#endif // MAINTABS_H
