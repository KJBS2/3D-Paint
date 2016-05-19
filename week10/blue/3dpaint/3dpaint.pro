#-------------------------------------------------
#
# Project created by QtCreator 2016-05-18T19:11:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3dpaint
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    objviewer.cpp \
    objwindow.cpp

HEADERS  += mainwindow.h \
    objviewer.h \
    objwindow.h

FORMS    += mainwindow.ui
