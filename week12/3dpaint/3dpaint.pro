#-------------------------------------------------
#
# Project created by QtCreator 2016-05-18T19:11:05
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3dpaint
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    objwindow.cpp \
    objviewer.cpp \

HEADERS  += mainwindow.h \
    objwindow.h \
    objviewer.h \

FORMS    += mainwindow.ui
