#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_released()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    this->ui->lineEdit->setText(fileName);
}

void MainWindow::on_openButton_released()
{
    this->image = QImage(this->ui->lineEdit->text());
    if(!this->image.isNull())
    {
        this->ui->beforeImageDice->setPixmap(QPixmap::fromImage(this->image.scaledToHeight(160)));
    }
}

void MainWindow::on_processDiceButton_released()
{
    QGrayImage grayImage;
    QBWImage bwImage;
    QBWImage processedImage;
    if(!this->image.isNull())
    {
        grayImage = QGrayImage(image);
        bwImage = grayImage.toBW(202);
        processedImage = bwImage.close(3);

        this->ui->afterImageDice->setPixmap(QPixmap::fromImage(processedImage.scaledToHeight(160)));
    }
}
