#ifndef INPUTKEYBOARDQT_H
#define INPUTKEYBOARDQT_H

#include <QObject>
#include <QWidget>
#include <QKeyEvent>
#include <QList>
#include <QMap>
#include <QLocale>
#include <QDebug>

#include "enums.h"

class InputKeyboardQt : public QObject
{
    Q_OBJECT
public:
    explicit InputKeyboardQt(QObject *parent = nullptr);
    
    bool callEventFilter(QObject *obj, QEvent *ev);
    
private:
    //QList<int> list_function_keys;
    
    bool ctrl_down = false;
    
    // Qt's "if (!event->isAutoRepeat())" does not work here, since
    // we use in "input_keyboard_xcb" the function "XkbSetDetectableAutoRepeat" to easily detect autorepeats.
    // Sadly this breaks Qts lesser fortunate and more overcomplicated way of doing it (Qt 5.13; Qt 6.3), so we have now to reimplement an "isAutoRepeat" ourselves in a way that acually makes sense.
    QList<int> list_of_keypresses;
    bool isAutoRepeat(QEvent *event, QKeyEvent *key);
    
    int mapFromKeyboardLayoutToLinuxRaw(int key);
    
    void sendUDPMessage(QString message);
    
protected:
    //bool eventFilter(QObject *obj, QEvent *ev) override;
    
signals:
    /*
    void panickSignal();
    void stopAllSignal();
    void showMenuSignal();
    void resendMIDISettingsSignal();
    */
    
    void keyPressSignal(int key);
    void keyReleaseSignal(int key);
    void toggleKeyboardLockSignal();
    //void changeTabSignal(int id);
    void MIDISignal(MIDISignalTypes type);
};

#endif // INPUTKEYBOARDQT_H
