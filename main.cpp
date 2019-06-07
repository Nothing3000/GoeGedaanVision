#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow *window = new MainWindow();

    window->setWindowTitle(QString::fromUtf8("GoeGedaan"));

    window->show();
    return app.exec();
}
