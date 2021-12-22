#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QSettings>

#include <QDebug>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    
    void setValue(QString key, QVariant value);
    void saveChannelSettings(QString label, QList<QMap<QString, QVariant> > channels);
    
private:
    QSettings *config;
    QSettings *settings;
    
    void openSettingsFile();
    QString getDbPath();
    void setDbPath(QString db_path);
    
signals:
    
};

#endif // CONFIG_H
