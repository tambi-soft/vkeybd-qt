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
#include "input_keyboard_raw.h"

class MainTabs : public QTabWidget
{
    Q_OBJECT
public:
    explicit MainTabs(int id, Config* config, QString output_system, QComboBox *combo_keyboard_input, QPushButton *button_lock, QPushButton *button_keyboard_rescan, QLineEdit *line_udp_ip, QSpinBox *spin_port, QTabWidget *parent = nullptr);
    
    bool callEventFilter(QObject *obj, QEvent *ev);
    void saveAllParams();
    void restoreParams(QString tab, QMap<QString,QVariant> data);
    
    void showHideGUIElements(QString name, bool show);
    
    void rawKeyPressed(int keycode);
    void rawKeyReleased(int keycode);
    
private:
    QList<int> list_function_keys;
    QList<int> list_function_keys_raw;
    QList<int> list_function_keys_native;
    QList<QString> list_labels;
    QList<Orgelwerk*> list_of_tabs;
    QMap<QString,Orgelwerk*> map_of_tabs;
    
    QMap<int,QString> combo_keyboard_input_labels;
    bool input_kbd_qt_native = false;
    bool input_kbd_qt_default = false;
    bool input_kbd_linux_raw = false;
    
    int id;
    Config *config;
    QComboBox *combo_keyboard_input;
    QPushButton *button_lock;
    QLineEdit *line_udp_ip;
    QSpinBox *spin_port;
    
    bool keyboard_locked = false;
    InputKeyboardRaw *keyboard_raw;
    bool ctrl_down = false;
    
    QUdpSocket *socket;
    void rebindSocketIP(QString ip);
    void rebindSocket(int value);
    bool send_udp = false;
    void sendUDPMessage(QString message);
    void receiveUDPMessage();
    
    void initializeTabs(QString output_system);
    void addOrganTab(QString output_system, QString label, int number_of_orgelwerks=1);

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    //bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    
private slots:
    void keyboardSelectionChanged(QString text);
    void deviceNotAvailable(QString message);
    void toggleKeyboardLock();
    void keyboardRescan();
    
signals:
    void useInputKbdQtNativeSignal();
    void useInputKbdQtDefaultSignal();
};

#endif // MAINTABS_H
