#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include "dice.h"
#include "plate.h"

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
        this->ui->beforeImagePlates->setPixmap(QPixmap::fromImage(this->image.scaledToHeight(160)));
    }
}

void MainWindow::on_processDiceButton_released()
{
    Dice dice(this->image);
    QString diceString;
    QTextStream stream(&diceString);
    this->ui->beforeImageDice->setPixmap(QPixmap::fromImage(dice.getMarkedImage()).scaledToHeight(160));
    this->ui->outputList->clear();
    for(int i = 0; i < dice.amountOfDice(); i++)
    {
        diceString.clear();
        stream << "Dice " << i+1 <<": " << dice.dotsOnDice(i);
        this->ui->outputList->addItem(diceString);
    }
}

void MainWindow::on_processPlateButton_released()
{
    Plate plate(this->image);
    this->ui->beforeImagePlates->setPixmap(QPixmap::fromImage(plate.getMarkedImage().scaledToHeight(160)));
    this->ui->afterImagePlates->setText(plate.getText());

}
