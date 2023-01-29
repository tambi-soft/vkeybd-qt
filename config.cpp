#include "config.h"

Config::Config(QObject *parent) : QObject(parent)
{
    this->config_dir = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QFile *config_file = new QFile();
    
    if (! config_dir->exists())
    {
        config_dir->mkdir(config_dir->absolutePath());
    }
    
    config_file->setFileName(config_dir->absolutePath() + "/config.ini");
    this->config = new QSettings(config_file->fileName(), QSettings::IniFormat);
    
    if (! config_file->exists())
    {
        this->config->setValue("default/quicksave-path-n1", config_dir->absoluteFilePath("quicksave_n1.ini"));
        this->config->setValue("default/number-of-keyboards", 1);
        this->config->setValue("default/number-of-layers", 2);
        this->config->setValue("default/output", "alsa");
        this->config->setValue("default/keyboard-config", "keyboard-default.json");
    }
}

QSettings *Config::openQuicksaveFile(int number_of_keyboards)
{
    QString path = "default/quicksave-path-n"+QString::number(number_of_keyboards);
    if (!this->config->contains(path))
    {
        this->config->setValue(path, config_dir->absoluteFilePath("quicksave_n"+QString::number(number_of_keyboards)+".ini"));
    }
    
    return new QSettings(this->config->value(path).toString(), QSettings::IniFormat);
}
QSettings *Config::openSaveFile(QString filepath)
{
    return new QSettings(filepath, QSettings::IniFormat);
}

void Config::saveLastSavePath(QString path)
{
    this->config->setValue("runtime/last_path", path);
}
QString Config::getLastSavePath()
{
    return this->config->value("runtime/last_path").toString();
}

/*
QString Config::getQuicksavePath()
{
    return this->config->value("default/quicksave-path").toString();
}
void Config::setQuicksavePath(QString db_path)
{
    this->config->setValue("default/quicksave-path", db_path);
}
*/

int Config::getNumberOfKeyboards()
{
    return this->config->value("default/number-of-keyboards").toInt();
}
void Config::setNumberOfKeyboards(int number)
{
    this->config->setValue("default/number-of-keyboards", number);
}

int Config::getNumberOfLayers()
{
    int result = this->config->value("default/number-of-layers").toInt();
    if (result == 0)
    {
        result = 2;
        setNumberOfLayers(result);
    }
    
    return result;
}
void Config::setNumberOfLayers(int number)
{
    this->config->setValue("default/number-of-layers", number);
}

QString Config::getKeyboardConfig()
{
    return this->config->value("default/keyboard-config").toString();
}
void Config::setKeyboardConfig(QString file)
{
    this->config->setValue("default/keyboard-config", file);
}

QString Config::getOutputSystem()
{
    return this->config->value("default/output").toString();
}
void Config::setOutputSystem(QString output)
{
    this->config->setValue("default/output", output);
}
/*
void Config::setValue(QString key, QVariant value)
{
    this->settings->setValue(key, value);
}
*/
void Config::saveChannelSettings(QSettings *settings, int id, QString label, QList<QMap<QString,QVariant>> channels)
{
    for (int c=0; c < channels.length(); c++)
    {
        QList<QString> keys = channels.at(c).keys();
        
        for (int k=0; k < keys.length(); k++)
        {
            QString key = keys.at(k);
            settings->setValue(
                        QString::number(id)+"/"+label+"/"+QString::number(c)+"/"+key,
                        channels.at(c)[key]
                    );
        }
    }
}
void Config::saveParams(QSettings *settings, int id, QString label, QString channel, QMap<QString,QVariant> params)
{
    for (int i=0; i < params.size(); i++)
    {
        QString key = params.keys().at(i);
        
        QString path = QString::number(id)+"/"+label+"/"+key;
        if (! channel.isEmpty())
        {
            path = QString::number(id)+"/"+label+"/"+channel+"/"+key;
        }
                
        settings->setValue(
                    path,
                    params[key]
                    );
    }
}

void Config::loadChannelSettings(QSettings *settings)
{
    // maintabs is something like ["0", "1"] if vkeybd-qt sarted with -n=2
    QList<QString> maintabs = settings->childGroups();
    for (auto &maintab : maintabs)
    {
        settings->beginGroup(maintab);
        // tabs is something like ["F1", "F2", ... "F12", "general"]
        QList<QString> tabs = settings->childGroups();
        settings->endGroup();
        
        for (auto &tab : tabs)
        {
            if (tab != "general")
            {
                QMap<QString, QVariant> channels_;
                
                settings->beginGroup(maintab+"/"+tab);
                // channels is something like ["1", "2", ... "16"]
                QList<QString> channels = settings->childGroups();
                settings->endGroup();
                
                for (auto &channel : channels)
                {
                    settings->beginGroup(maintab+"/"+tab+"/"+channel);
                    // keys is something like ["tremolo", "volume", "msb", "lsb" ...]
                    QList<QString> keys = settings->childKeys();
                    settings->endGroup();
                    
                    QMap<QString, QVariant> value_;
                    
                    for (auto &key : keys)
                    {
                        QVariant value = settings->value(maintab+"/"+tab+"/"+channel+"/"+key);
                        
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
                settings->beginGroup(maintab+"/"+tab);
                QList<QString> generals = settings->childKeys();
                settings->endGroup();
                
                QMap<QString,QVariant> generals_;
                for (auto &general : generals)
                {
                    QVariant value = settings->value(maintab+"/"+tab+"/"+general);
                    generals_[general] = value;
                }
                
                emit restoreGeneral(maintab.toInt(), generals_);
            }
        }
    }
}
