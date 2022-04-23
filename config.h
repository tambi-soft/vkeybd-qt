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
    void saveChannelSettings(int id, QString label, QList<QMap<QString, QVariant> > channels);
    void loadChannelSettings();
    void saveParams(int id, QString label, QString channel, QMap<QString,QVariant> params);
    
private:
    QSettings *config;
    QSettings *settings;
    
    void openSettingsFile();
    QString getQuicksavePath();
    void setQuicksavePath(QString db_path);
    
signals:
    void restoreParams(int maintab, QString tab, QMap<QString, QVariant> data);
    void restoreGeneral(int maintab, QMap<QString, QVariant> data);
};

#endif // CONFIG_H
