#ifndef MAINTABS_H
#define MAINTABS_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>

#include <QUdpSocket>
#include <QCoreApplication>

#include "orgelwerk.h"
#include "config.h"
#include "enums.h"

#include "input_keyboard_select.h"
//#include "combo_keyboard_select.h"

#include "enums.h"

class MainTabs : public QTabWidget
{
    Q_OBJECT
public:
    explicit MainTabs(int id, Config* config, OutputSystem output, InputKeyboardSelect *input_keyboard_select, QLineEdit *line_udp_ip, QSpinBox *spin_port, QTabWidget *parent = nullptr);
    
    //bool callEventFilter(QObject *obj, QEvent *ev);
    void saveAllParams();
    void restoreParams(QString tab, QMap<QString,QVariant> data);
    
    void showHideGUIElements(GUIElements elements, bool show);
    
    void rawKeyPressed(int keycode);
    void rawKeyReleased(int keycode);
    void changeTab(int id);
    void MIDISignal(MIDISignalTypes type);
    
private:
    //QList<int> list_function_keys;
    QList<int> list_function_keys_raw;
    //QList<int> list_function_keys_native;
    QList<QString> list_labels;
    QList<Orgelwerk*> list_of_tabs;
    QMap<QString,Orgelwerk*> map_of_tabs;
    
    //bool input_kbd_qt_native = false;
    //bool input_kbd_qt_default = false;
    //bool input_kbd_linux_raw = false;
    
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
    void addOrganTab(OutputSystem output, QString label, int number_of_orgelwerks=1);

protected:
    //bool eventFilter(QObject *obj, QEvent *ev) override;
    
private slots:
    //void keyboardSelectionChanged(QString text);
    void deviceNotAvailable(QString message);
    //void toggleKeyboardLock();
   
    
signals:
    void useInputKbdQtNativeSignal();
    void useInputKbdQtDefaultSignal();
    
};

#endif // MAINTABS_H
