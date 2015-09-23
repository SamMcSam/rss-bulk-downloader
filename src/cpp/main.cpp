#include "../h/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Sam");
    a.setApplicationName("RSS Bulk Downloader");

    MainWindow w;
    w.show();

    return a.exec();
}
