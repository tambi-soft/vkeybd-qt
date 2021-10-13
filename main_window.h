#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include "orgelwerk.h"
#include "keyboard_piano.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    
private:
    KeyboardPiano *piano;
    Orgelwerk *werk;
    
    QMap<QString, QString> key_keycode_map = {};
    
protected:
    //bool eventFilter(QObject *obj, QEvent *ev);
    
};
#endif // MAINWINDOW_H
