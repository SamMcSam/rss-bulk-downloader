#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QMessageBox>
#include <QTreeWidget>
#include <QtNetwork>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QStringList>
#include <QDomDocument>

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots :
    void reset();
    void howTo();
    void about();
    void choosePath();
    void download();
    void crawl(QNetworkReply *reply);
    void downloadFiles(QNetworkReply *reply);
    void downloadNext();

private:
    Ui::MainWindow *ui;
    int nbrEpisodes;
    int nbrEpisodesDownloaded;
    QStringList mp3s;
};

#endif // MAINWINDOW_H
