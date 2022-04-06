#include "main_window.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationVersion("0.5");
    QApplication::setApplicationName("vkeybd-qt");
    
    
    QCommandLineParser parser;
    parser.setApplicationDescription("vkeybd-qt is a powerful and most feature-complete software midi controller");
    parser.addHelpOption();
    //parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to copy."));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));
    
    parser.addOptions({
        {"sattelite",
            QCoreApplication::translate("main", "Show windows in sattelite mode to remote control another instance of vkeybd-qt over the network.")},
        {{"n", "number-of-keyboards"},
            QCoreApplication::translate("main", "Number of Keyboards. Should not be too high, because your soundsystem may be overwhelmed.")}
    });
    
    parser.process(app);
    
    bool is_sattelite = parser.isSet("sattelite");
    int number_of_keyboards = parser.value("number-of-keyboards").toInt();
    if (number_of_keyboards <= 0)
    {
        number_of_keyboards = 1;
    }
    else if (number_of_keyboards > 10)
    {
        number_of_keyboards = 10;
    }
    
    MainWindow win(is_sattelite, number_of_keyboards);
    win.show();
    return app.exec();
}
