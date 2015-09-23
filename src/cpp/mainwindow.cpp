#include "../h/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //menu
    connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(reset()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
    connect(ui->actionHow_To, SIGNAL(triggered()), this, SLOT(howTo()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    //button
    connect(ui->downloadButton, SIGNAL(clicked()), this, SLOT(download()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset()
{
    qDebug() << "Reset was pressed";
}

void MainWindow::quit()
{
    qDebug() << "Quit was pressed";
}

void MainWindow::howTo()
{
    qDebug() << "How to was pressed";
}

void MainWindow::about()
{
    qDebug() << "About was pressed";

}

void MainWindow::download()
{
    qDebug() << "Download was pressed";

}
