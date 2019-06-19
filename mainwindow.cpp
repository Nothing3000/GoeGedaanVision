#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QTextStream>

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
    QString outText;
    QTextStream outTextStream(&outText);
    QBWImage::objectsVector dice;
    int totalDots = 0;
    int amountOfDice = 0;
    QGrayImage grayImage;
    QBWImage bwImage;
    QBWImage processedImage;
    QBWImage dieImage;
    if(!this->image.isNull())
    {
        grayImage = QGrayImage(image);
        bwImage = grayImage.toBW(202);
        processedImage = processedImage.close(7);
        processedImage = bwImage.areaopen(1900);
        dice = processedImage.conncomp();

        for(auto die : dice)
        {
            outText.clear();
            int dots;
            amountOfDice++;
            dieImage = ~QBWImage(processedImage.width(), processedImage.height(), die);
            dots = dieImage.conncomp().size()-1;
            totalDots += dots;
            outTextStream << "Die " << amountOfDice << ": " << dots;
            this->ui->outputList->addItem(outText);
        }
        outText.clear();
        outTextStream << "Total: " << totalDots;
        this->ui->outputList->addItem(outText);
        this->ui->beforeImageDice->setPixmap(QPixmap::fromImage(processedImage.scaledToHeight(160)));

    }
}
