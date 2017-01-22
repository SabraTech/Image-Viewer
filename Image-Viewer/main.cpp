#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(MainWindow::("Image Viewer"));
    QCommandLineParser commandParser;
    commandParser.addHelpOption();
    commandParser.addPositionalArgument(MainWindow::tr("[file]"),MainWindow::("Image file to open."));
    commandParser.process(QCoreApplication::agruments());
    MainWindow window;

    if(!commandParser.positionalArguments().isEmpty() && !MainWindow.loadFile(commandParser.positionalArguments().front())){
      return -1;
    }
    window.show();
    return app.exec();
}
