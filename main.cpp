#include "main_window.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationVersion("0.5");
    
    
    QCommandLineParser parser;
    parser.setApplicationDescription("vkeybd-qt is a powerful and most feature-complete software midi controller");
    parser.addHelpOption();
    //parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to copy."));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));
    
    // A boolean option with a single name (-p)
    QCommandLineOption viewAsSatteliteOption("sattelite", QCoreApplication::translate("main", "show windows in sattelite mode to remote control another instance of vkeybd-qt over the network"));
    parser.addOption(viewAsSatteliteOption);
    
    parser.process(app);
    
    
    bool is_sattelite = parser.isSet(viewAsSatteliteOption);
    
    MainWindow win(is_sattelite);
    win.show();
    return app.exec();
}
