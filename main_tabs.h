#ifndef MAINTABS_H
#define MAINTABS_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpinBox>

#include <QUdpSocket>
#include <QCoreApplication>

#include "orgelwerk.h"
#include "config.h"

class MainTabs : public QTabWidget
{
    Q_OBJECT
public:
    explicit MainTabs(int id, Config* config, QString output_system, QLineEdit *line_udp_ip, QSpinBox *spin_port, QTabWidget *parent = nullptr);
    
    bool callEventFilter(QObject *obj, QEvent *ev);
    void saveAllParams();
    //void openAllParams(int id);
    void restoreParams(QString tab, QMap<QString,QVariant> data);
    
private:
    QList<int> list_function_keys;
    QList<QString> list_labels;
    QList<Orgelwerk*> list_of_tabs;
    QMap<QString,Orgelwerk*> map_of_tabs;
    
    int id;
    Config *config;
    QLineEdit *line_udp_ip;
    QSpinBox *spin_port;
    
    QUdpSocket *socket;
    void rebindSocketIP(QString ip);
    void rebindSocket(int value);
    bool send_udp = false;
    void sendUDPMessage(QString message);
    void receiveUDPMessage();
    
    void initializeTabs(QString output_system);
    void addOrganTab(QString output_system, QString label, int number_of_orgelwerks=1);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    
private slots:
    
signals:
    
};

#endif // MAINTABS_H
