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
        
        this->config->setValue("default/quicksave_path", config_dir->absoluteFilePath("quicksave.ini"));
    }
    
    openSettingsFile();
}

void Config::openSettingsFile()
{
    this->settings = new QSettings(this->config->value("default/quicksave_path").toString(), QSettings::IniFormat);
}

QString Config::getQuicksavePath()
{
    return this->settings->value("default/quicksave_path").toString();
}

void Config::setQuicksavePath(QString db_path)
{
    this->settings->setValue("default/quicksave_path", db_path);
}

void Config::setValue(QString key, QVariant value)
{
    this->settings->setValue(key, value);
}

void Config::saveChannelSettings(int id, QString label, QList<QMap<QString,QVariant>> channels)
{
    for (int c=0; c < channels.length(); c++)
    {
        QList<QString> keys = channels.at(c).keys();
        
        for (int k=0; k < keys.length(); k++)
        {
            QString key = keys.at(k);
            this->settings->setValue(
                        QString::number(id)+"/"+label+"/"+QString::number(c)+"/"+key,
                        channels.at(c)[key]
                    );
        }
    }
}
void Config::saveParams(int id, QString label, QString channel, QMap<QString,QVariant> params)
{
    for (int i=0; i < params.size(); i++)
    {
        QString key = params.keys().at(i);
        
        QString path = QString::number(id)+"/"+label+"/"+key;
        if (! channel.isEmpty())
        {
            path = QString::number(id)+"/"+label+"/"+channel+"/"+key;
        }
                
        this->settings->setValue(
                    path,
                    params[key]
                    );
    }
}

void Config::loadChannelSettings()
{
    // maintabs is something like ["0", "1"] if vkeybd-qt sarted with -n=2
    QList<QString> maintabs = this->settings->childGroups();
    for (auto &maintab : maintabs)
    {
        this->settings->beginGroup(maintab);
        // tabs is something like ["F1", "F2", ... "F12", "general"]
        QList<QString> tabs = this->settings->childGroups();
        this->settings->endGroup();
        
        for (auto &tab : tabs)
        {
            if (tab != "general")
            {
                QMap<QString, QVariant> channels_;
                
                this->settings->beginGroup(maintab+"/"+tab);
                // channels is something like ["1", "2", ... "16"]
                QList<QString> channels = this->settings->childGroups();
                this->settings->endGroup();
                
                for (auto &channel : channels)
                {
                    this->settings->beginGroup(maintab+"/"+tab+"/"+channel);
                    // keys is something like ["tremolo", "volume", "msb", "lsb" ...]
                    QList<QString> keys = this->settings->childKeys();
                    this->settings->endGroup();
                    
                    QMap<QString, QVariant> value_;
                    
                    for (auto &key : keys)
                    {
                        QVariant value = this->settings->value(maintab+"/"+tab+"/"+channel+"/"+key);
                        
                        value_[key] = value; // e.g: "volume" -> "127"
                    }
                    
                    channels_[channel] = value_; // e.g: "1" -> ("volume" -> "127")
                }
                
                emit restoreParams(
                            maintab.toInt(),
                            tab,
                            channels_
                            );
            }
            else if (tab == "general")
            {
                this->settings->beginGroup(maintab+"/"+tab);
                QList<QString> generals = this->settings->childKeys();
                this->settings->endGroup();
                
                QMap<QString,QVariant> generals_;
                for (auto &general : generals)
                {
                    QVariant value = this->settings->value(maintab+"/"+tab+"/"+general);
                    generals_[general] = value;
                }
                
                emit restoreGeneral(maintab.toInt(), generals_);
            }
        }
    }
}
