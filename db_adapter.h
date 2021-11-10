#ifndef DBADAPTER_H
#define DBADAPTER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

class DbAdapter : public QObject
{
    Q_OBJECT
public:
    explicit DbAdapter(QObject *parent = nullptr);
    
signals:
    
};

#endif // DBADAPTER_H
