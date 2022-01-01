#include "config.h"

Config::Config(QObject *parent) : QObject(parent)
{
    QDir *config_dir = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QFile *config_file = new QFile();
    
    if (! config_dir->exists())
    {
        config_dir->mkdir(config_dir->absolutePath());
    }
    
    config_file->setFileName(config_dir->absolutePath() + "/config.ini");
    this->config = new QSettings(config_file->fileName(), QSettings::IniFormat);
    
    if (! config_file->exists())
    {
        /*
        FirstRun *run = new FirstRun("");
        connect(run, &FirstRun::databasePathSelected, this, &Config::setDbPath);
        run->exec();
        */
        
        this->config->setValue("default/db_path", config_dir->absoluteFilePath("default.ini"));
    }
    
    openSettingsFile();
}

void Config::openSettingsFile()
{
    this->settings = new QSettings(this->config->value("default/db_path").toString(), QSettings::IniFormat);
}

QString Config::getDbPath()
{
    return this->settings->value("default/db_path").toString();
}

void Config::setDbPath(QString db_path)
{
    this->settings->setValue("default/db_path", db_path);
}

void Config::setValue(QString key, QVariant value)
{
    this->settings->setValue(key, value);
}

void Config::saveChannelSettings(QString label, QList<QMap<QString,QVariant>> channels)
{
    for (int c=0; c < channels.length(); c++)
    {
        QList<QString> keys = channels.at(c).keys();
        
        for (int k=0; k < keys.length(); k++)
        {
            QString key = keys.at(k);
            
            this->settings->setValue(
                        //label+"_"+QString::number(c)+"/"+key,
                        label+"/"+QString::number(c)+"/"+key,
                        channels.at(c)[key]
                    );
        }
    }
}

QList<QMap<QString,QVariant>> Config::loadChannelSettings(QString tab_label)
{
    QList<QMap<QString,QVariant>> result;
    
    //qDebug() << this->settings->allKeys();
    //QStringList child_groups = this->settings->childGroups();
    
    int last_tab_channel_id = 0;
    QMap<QString,QVariant> map;
    QStringList keys = this->settings->allKeys();
    for (int i=0; i < keys.length(); i++)
    {
        QStringList splitted = keys.at(i).split("/");
        QString tab_label_ = splitted.at(0); // "F1", "F2" ...
        int tab_channel_id = splitted.at(1).toInt(); // 0, 1, ..., 15
        QString tab_channel_key = splitted.at(2); // "key_min", "key_shift", ...
        
        if (tab_label == tab_label_)
        {
            if (last_tab_channel_id == tab_channel_id)
            {
                map[tab_channel_key] = this->settings->value(keys.at(i));
            }
            else
            {
                result.append(map);
                map.clear();
            }
            //map["channel"] = tab_channel_id;
            //map[tab_channel_key] = this->settings->value(keys.at(i));
        }
        
        //result.append(map);
        
        last_tab_channel_id = tab_channel_id;
    }
    result.append(map);
    
    qDebug() << result;
    
    return result;
}
