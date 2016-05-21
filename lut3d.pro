#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T21:16:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lut3d
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        lut3d.cpp \
    image.cpp \
    color.cpp

HEADERS  += mainwindow.h \
    lut3d.h \
    image.h \
    color.h

FORMS    += mainwindow.ui
