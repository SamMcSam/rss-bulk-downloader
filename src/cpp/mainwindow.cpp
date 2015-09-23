#include "../h/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //menu
    connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(reset()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionHow_To, SIGNAL(triggered()), this, SLOT(howTo()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    //button
    connect(ui->downloadButton, SIGNAL(clicked()), this, SLOT(download()));

    reset();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset()
{
    ui->rssEdit->setText("");
    ui->result->setText(tr("Enter the RSS feed URL in the text field"));
}

void MainWindow::howTo()
{
    QMessageBox::about(this, tr("How to use"),
        tr("<ol>"
        "<li>Copy/paste the rss feed url in the text field</li>"
        "<li>Press download button, choose where to save</li>"
        "<li>Wait for it to finish downloading</li>"
        "<li>Enjoy :)</li>"
        "</ol>")
    );
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About"),
        tr("<h3>RSS Bulk Downloader</h3>"
        "<p>By Sam (2015)</p>"
        "<p>Made in C++ with QtCreator for the UI biznatch</p>"
        "<p>and libcurl library for the internet stuff</p>")
    );
}

void MainWindow::download()
{
    qDebug() << "Download was pressed";

    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose path to save downloads"),
        "/home",
        QFileDialog::ShowDirsOnly
        | QFileDialog::DontResolveSymlinks);


    qDebug() << dir;

    /*
     * channel
     * - item
     * -- enclosure url="..."
    */

    // copy QString to char*
    QString rssURL = ui->rssEdit->text();


    QUrl url(ui->rssEdit->text());



/*
    ui->result->setText(tr("Where do you want to save?"));
    ui->result->setText(tr("Downloading files..."));
    ui->result->setText(tr("Downloading files (0 of 10)..."));
    ui->result->setText(tr("Done! (Downloaded 0 of 10 files)"));
    */
}
