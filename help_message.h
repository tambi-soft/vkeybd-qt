#ifndef HELPMESSAGE_H
#define HELPMESSAGE_H

#include <QObject>
#include <QWidget>

#include <QObject>
#include <QWidget>
#include <QDialog>

#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QPushButton>

class HelpMessage : public QDialog
{
    Q_OBJECT
public:
    explicit HelpMessage(QString filename, QDialog *parent = nullptr);
    
signals:
    
};

#endif // HELPMESSAGE_H
