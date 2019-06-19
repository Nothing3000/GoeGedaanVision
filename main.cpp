#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow *window = new MainWindow();
    int exitCode;

    window->setWindowTitle(QString::fromUtf8("GoeGedaan"));

    window->show();
    exitCode = app.exec();
    delete window;
    return exitCode;
}
