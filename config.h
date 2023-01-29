#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QSettings>

#include <QDebug>

#include "enums_structs.h"

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    
    //void setValue(QString key, QVariant value);
    void saveChannelSettings(QSettings *settings, int id, QString label, QList<QMap<QString, QVariant> > channels);
    void loadChannelSettings(QSettings *settings);
    void saveParams(QSettings *settings, int id, QString label, QString channel, QMap<QString,QVariant> params);

    QSettings *openQuicksaveFile(int number_of_keyboards);
    QSettings *openSaveFile(QString filepath);
    void saveLastSavePath(QString path);
    QString getLastSavePath();
    
    int getNumberOfKeyboards();
    void setNumberOfKeyboards(int number);
    
    int getNumberOfLayers();
    void setNumberOfLayers(int number);
    
    QString getOutputSystem();
    void setOutputSystem(QString output);
    
    QString getKeyboardConfig();
    void setKeyboardConfig(QString file);
    
private:
    QDir *config_dir;
    
    // this is the main config.ini
    QSettings *config;
    // this is the file we use to save/restore our saved presets
    //QSettings *settings;
    
signals:
    void restoreParams(int maintab, QString tab, QMap<QString, QVariant> data);
    void restoreGeneral(int maintab, QMap<QString, QVariant> data);
};

#endif // CONFIG_H
