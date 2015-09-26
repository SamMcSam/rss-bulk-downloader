#include "../h/mainwindow.h"
#include "ui_mainwindow.h"

//@TODO test if empty dir, empty rss...

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
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
    connect(ui->dirButton, SIGNAL(clicked()), this, SLOT(choosePath()));

    reset();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::reset
 */
void MainWindow::reset()
{
    ui->rssEdit->setText("");
    ui->result->setText(tr("Enter the RSS feed URL in the text field"));
    ui->dirEdit->setText("C:/");

    nbrEpisodes = 0;
}

/**
 * @brief MainWindow::howTo
 */
void MainWindow::howTo()
{
    QMessageBox::information(this, tr("How to use"),
        tr("<ol>"
        "<li>Copy/paste the rss feed url in the text field, choose where to save</li>"
        "<li>Press download button</li>"
        "<li>Wait for it to finish downloading</li>"
        "<li>Enjoy :)</li>"
        "</ol>")
    );
}

/**
 * @brief MainWindow::about
 */
void MainWindow::about()
{
    QMessageBox::about(this, tr("About"),
        tr("<h3>RSS Bulk Downloader</h3>"
        "<p>By Sam (2015)</p>"
        "<p>Made in C++ with QtCreator for the UI and network biznatch</p>")
    );
}

/**
 * @brief MainWindow::choosePath
 */
void MainWindow::choosePath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose path to save downloads"),
        "/home",
        QFileDialog::ShowDirsOnly
        | QFileDialog::DontResolveSymlinks);

    ui->dirEdit->setText(dir);
}

/**
 * @brief MainWindow::download
 */
void MainWindow::download()
{
    QString rssURL = ui->rssEdit->displayText();

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(rssURL));
    manager->get(request);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(crawl(QNetworkReply*)));

    ui->result->setText(tr("Connecting..."));
}

/**
 * @brief MainWindow::crawl
 * @param reply
 */
void MainWindow::crawl(QNetworkReply *reply)
{
    QDomDocument dom;
    dom.setContent(reply->readAll());

    //get items
    QDomNodeList items = dom.elementsByTagName("item");
    nbrEpisodes = items.size();

    //get all mp3s urls
    QDomNodeList enclosures = dom.elementsByTagName("enclosure");
    QDomElement enclosure;
    std::list<QString> mp3s;

    for (int i=0; i<enclosures.size(); i++) {
        enclosure = enclosures.item(i).toElement();
        mp3s.push_front(enclosure.attribute("url"));
        qDebug() << enclosure.attribute("url");
    }
    qDebug() << mp3s.size();

    ui->result->setText(tr("Data received! Start crawler..."));
}
