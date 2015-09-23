#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T20:13:02
#
#-------------------------------------------------

QT       += core\
        gui\
        xml\
        network\
        webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rss-bulk-downloader
TEMPLATE = app

SOURCES += src/cpp/mainwindow.cpp \
        src/cpp/main.cpp

HEADERS  += src/h/mainwindow.h

FORMS    += src/ui/mainwindow.ui


OTHER_FILES += \
    README.md

