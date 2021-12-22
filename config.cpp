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
