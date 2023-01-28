#include "main_window.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <QDebug>

#include "config.h"
#include "enums_structs.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationVersion("0.5");
    QApplication::setApplicationName("vkeybd-qt");
    //qDebug() << 
    //app.inputMethod()->show();
    
    Config *config = new Config;
    int config_number_of_keyboards = config->getNumberOfKeyboards();
    QString config_output_system = config->getOutputSystem();
    QString config_keyboard_config = config->getKeyboardConfig();
    delete config;
    
    QCommandLineParser parser;
    parser.setApplicationDescription("vkeybd-qt is a powerful and most feature-complete software midi controller");
    parser.addHelpOption();
    //parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to copy."));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));
    
    parser.addOptions({
        {{"n", "number-of-keyboards"},
            QCoreApplication::translate("main", "Number of Keyboards. Should not be too high, because your soundsystem may be overwhelmed."), "number", "-1"},
        {{"o", "output"},
            QCoreApplication::translate("main", "Which Audio System to use: \"alsa\" (default), \"jack\" (not implemented yet) or \"network\" (remote control another vkeybd-qt instance)"), "string", "config"},
        {{"k", "keyboard-config"},
            QCoreApplication::translate("main", "Keyboard Configs and Mappings to use"), "string", "builtin"},
        {{"q", "quicksave-path"},
            QCoreApplication::translate("main", "Path for the quicksave/quickload file"), "string", "default"}
    });
    
    parser.process(app);
    
    
    int number_of_keyboards = parser.value("number-of-keyboards").toInt();
    if (number_of_keyboards == -1)
    {
        number_of_keyboards = config_number_of_keyboards;
    }
    
    if (number_of_keyboards <= 0)
    {
        number_of_keyboards = 1;
        
        fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main", "ERROR: --number-of-keyboards can not be zero or negative. Defaulting to \"1\".")));
    }
    else if (number_of_keyboards > 10)
    {
        number_of_keyboards = 10;
        
        fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main", "ERROR: --number-of-keyboards higher than 10 is probably not a good idea. Defaulting to \"10\".")));
    }
    
    QString output_system = QString::fromStdString(parser.value("output").toStdString());
    if (output_system == "config")
    {
        output_system = config_output_system;
    }
    
    if ((output_system != "alsa") & (output_system != "jack") & (output_system != "network"))
    {
        fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main", "ERROR: Option \"-o\" / \"--output\" has to be one of: \"alsa\", \"jack\" or \"network\".")));
        fprintf(stderr, "\n");
        
        // showHelp also stops program execution
        parser.showHelp(1);
    }
    else
    {
        OutputSystem output;
        output = OutputSystem::Alsa;
        if (output_system == "alsa")
            output = OutputSystem::Alsa;
        else if (output_system == "jack")
            output = OutputSystem::Jack;
        else if (output_system == "network")
            output = OutputSystem::Network;
        
        MainWindow win(output, number_of_keyboards);
        win.show();
        return app.exec();
    }
}
