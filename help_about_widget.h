#ifndef HELPABOUTWIDGET_H
#define HELPABOUTWIDGET_H

#include <QObject>
#include <QWidget>

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>

class HelpAboutWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HelpAboutWidget(QWidget *parent = nullptr);
    
private:
    QVBoxLayout *layout;
    QTabWidget *tab_widget;
    void showAbout();
    void showLibraries();
    void showAuthors();
    void showThanks();
    void showLicense();
    
signals:
    

public slots:
    
};

#endif // HELPABOUTWIDGET_H
