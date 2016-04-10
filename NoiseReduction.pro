#-------------------------------------------------
#
# Project created by QtCreator 2016-04-10T13:40:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NoiseReduction
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    AudioEngine.cpp

HEADERS  += mainwindow.h \
    AudioEngine.h

FORMS    += mainwindow.ui
