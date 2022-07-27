#ifndef MIDIKEYSHIFTGLOBAL_H
#define MIDIKEYSHIFTGLOBAL_H

#include <QObject>
#include <QWidget>

#include <QGridLayout>
#include <QFrame>
#include <QFile>
#include <QLabel>

#include "midi_key_shift_widget.h"

class MIDIKeyShiftGlobal : public QWidget
{
    Q_OBJECT
public:
    explicit MIDIKeyShiftGlobal(QWidget *parent = nullptr);
    
private:
    int last_relative_pitch = 0;
    
    void valueChanged(int value, bool is_relative);
    
signals:
    void signalKeyShiftChanged(int value, bool is_relative);
};

#endif // MIDIKEYSHIFTGLOBAL_H
