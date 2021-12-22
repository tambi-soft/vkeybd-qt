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
    this->settings = new QSettings(config_file->fileName(), QSettings::IniFormat);
    
    if (! config_file->exists())
    {
        /*
        FirstRun *run = new FirstRun("");
        connect(run, &FirstRun::databasePathSelected, this, &Config::setDbPath);
        run->exec();
        */
        
        this->settings->setValue("default/db_path", config_dir->absoluteFilePath("default.ini"));
    }
    
    openConfigFile();
}

void Config::openConfigFile()
{
    
}
