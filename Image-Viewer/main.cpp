#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(MainWindow::tr("Image Viewer"));
    QCommandLineParser commandParser;
    commandParser.addHelpOption();
    commandParser.addPositionalArgument(MainWindow::tr("[file]"),MainWindow::tr("Image file to open."));
    commandParser.process(QCoreApplication::arguments());
    MainWindow window;

    if (!commandParser.positionalArguments().isEmpty()
            && !window.loadFile(commandParser.positionalArguments().front())) {
            return -1;
        }
    window.show();
    return app.exec();
}
