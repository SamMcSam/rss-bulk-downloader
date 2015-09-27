#include "../h/mainwindow.h"
#include "ui_mainwindow.h"

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
    nbrEpisodesDownloaded = 0;
    mp3s.clear();
    m_mp3Names.clear();

    //m_mp3file->reset();
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
    QUrl rssFeed = QUrl(rssURL);

    if (!rssFeed.isValid()) {
        ui->result->setText(tr("Enter valid URL!"));
        return;
    }

    m_directory = ui->dirEdit->displayText() + "/";
    if (!QDir(m_directory).exists()) {
        ui->result->setText(tr("Enter valid directory!"));
        return;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(rssFeed);
    manager->get(request);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(crawl(QNetworkReply*)));
    //connect(manager, SIGNAL(error(QNetworkReply*)), this, SLOT(f(QNetworkReply*))); //@TODO

    ui->result->setText(tr("Connecting..."));
}

/**
 * @brief MainWindow::crawl
 * @param reply
 */
void MainWindow::crawl(QNetworkReply *reply)
{
    if (reply->NoError == 0) {

        ui->result->setText(tr("Data received! Start crawler..."));

        QDomDocument dom;
        dom.setContent(reply->readAll());

        //get items
        QDomNodeList items = dom.elementsByTagName("item");
        nbrEpisodes = items.size();

        if (nbrEpisodes < 1) {
            ui->result->setText(tr("Couldn't fetch episodes. Not an rss feed?"));
            return;
        }

        //get all mp3s urls
        QDomNodeList enclosures = dom.elementsByTagName("enclosure");
        QDomElement enclosure;

        for (int i=0; i<enclosures.size(); i++) {
            enclosure = enclosures.item(i).toElement();
            mp3s.append(enclosure.attribute("url"));
            m_mp3Names.append(mp3s.last().split('/').last());
            qDebug() << enclosure.attribute("url");
        }

        if (mp3s.size() < 1) {
            ui->result->setText(tr("No episode files found."));
            return;
        }

        //cleanup
        reply->close();
        reply->deleteLater();

        downloadNext();
    }
    else {
        ui->result->setText(tr("Connection error"));
    }
}

/**
 * @brief MainWindow::saveFile
 * @param reply
 */
void MainWindow::saveFile()
{
    qDebug() << "SAVING FILE...";

    if (m_mp3file->NoError == 0) {

        // redirection!
        if (m_mp3file->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 302
            || m_mp3file->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 301) {

            qDebug() << ">> redirected!";
            qDebug() << "--- old:" << mp3s.front();
            mp3s.front() = m_mp3file->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
            qDebug() << "--- new:" << mp3s.front();
            downloadNext();
            return;
        }

        //file name
        QString fileName = m_mp3Names.at(nbrEpisodesDownloaded);

        qDebug() << fileName;
        qDebug() << (m_directory + fileName);

        //saving to disk
        QFile file(m_directory + fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(m_mp3file->readAll());
            file.close();
            nbrEpisodesDownloaded++;
        }
    }
    else {
        qDebug() << m_mp3file->errorString();
    }

    mp3s.pop_front();

    if (mp3s.size() > 0) {
        downloadNext();
        return;
    }
    else {
        ui->result->setText(
            QString("Done! (downloaded ") +
            QString::number(nbrEpisodes) +
            QString(" files out of ") +
            QString::number(nbrEpisodes) +
            QString(")")
        );
    }

    m_mp3file->close();
    m_mp3file->deleteLater();
}

/**
 * @brief MainWindow::downloadNext
 */
void MainWindow::downloadNext()
{
    qDebug() << mp3s.front();

    //prepare network
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(mp3s.front()));
    m_mp3file = manager->get(request);

    //connect(reply, SIGNAL(readyRead()), this, SLOT(readingReadyBytes()));
    //connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFiles(QNetworkReply*)));

    connect(m_mp3file, SIGNAL(finished()), this, SLOT(saveFile()));

    ui->result->setText(
        QString("Downloading file... (") +
        QString::number(nbrEpisodes - mp3s.size() + 1) +
        QString("/") +
        QString::number(nbrEpisodes) +
        QString(")")
    );
}
