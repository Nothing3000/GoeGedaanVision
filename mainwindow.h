#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_browseButton_released();

    void on_openButton_released();

    void on_processDiceButton_released();

    void on_processPlateButton_released();

private:
    Ui::MainWindow *ui;
    QImage image;
};

#endif // MAINWINDOW_H
