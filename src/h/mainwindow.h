#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QMessageBox>
#include <QTreeWidget>
#include <QXmlStreamReader>
#include <QtNetwork>
#include <QList>

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
    void download();
    void choosePath();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
