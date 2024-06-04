#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>  // Dodajemy brakujący nagłówek

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("TicTacToe Game");
    parser.addHelpOption();

    QCommandLineOption portOption(QStringList() << "p" << "port",
                                  QCoreApplication::translate("main", "Port to use for server/client."),
                                  QCoreApplication::translate("main", "port"),
                                  "12345");
    parser.addOption(portOption);

    parser.process(a);

    bool ok;
    int port = parser.value(portOption).toInt(&ok);
    if (!ok || port <= 0 || port > 65535) {
        qCritical() << "Invalid port number.";
        return 1;
    }

    MainWindow w(nullptr, port);
    w.show();
    return a.exec();
}

