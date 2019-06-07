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
    if(this->image) delete this->image;
    delete ui;
}

void MainWindow::on_browseButton_released()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    this->ui->lineEdit->setText(fileName);
}

void MainWindow::on_openButton_released()
{
    this->image = new QImage(this->ui->lineEdit->text());
    if(this->image->isNull())
    {
        delete this->image;
    }
}

void MainWindow::on_processDiceButton_released()
{
    if(this->image != nullptr)
    {
        if(!this->image->isNull())
        {
            this->ui->beforeImageDice->setPixmap(QPixmap::fromImage(*this->image));
        }
    }
}
