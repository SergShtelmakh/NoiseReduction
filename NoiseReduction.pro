#-------------------------------------------------
#
# Project created by QtCreator 2016-04-10T13:40:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NoiseReduction
TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp\
        mainwindow.cpp \
        AudioEngine.cpp

HEADERS  += mainwindow.h \
        AudioEngine.h

FORMS    += mainwindow.ui

LIBS += $$PWD/libs/libAquila.a\
        $$PWD/libs/libfftw3-3.dll\
        $$PWD/libs/wavelet2d.dll

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs
