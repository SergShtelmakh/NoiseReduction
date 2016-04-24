#-------------------------------------------------
#
# Project created by QtCreator 2016-04-10T13:40:35
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = NoiseReduction
TEMPLATE = app

CONFIG   += C++11

SOURCES  += $$PWD/src/main.cpp\
            $$PWD/src/mainwindow.cpp \
            $$PWD/src/AudioEngine.cpp \
            $$PWD/src/AudioSignal.cpp \
            $$PWD/src/wavelets/Wavelet.cpp \
            $$PWD/src/PlotManager.cpp \
            $$PWD/src/AudioRecordWidget.cpp \
            $$PWD/src/wavelets/DiscretePeriodicWavelet.cpp \
            $$PWD/src/DenoisingManager.cpp

HEADERS  += $$PWD/src/mainwindow.h \
            $$PWD/src/AudioEngine.h \
            $$PWD/src/AudioSignal.h \
            $$PWD/src/wavelets/Wavelet.h \
            $$PWD/src/PlotManager.h \
            $$PWD/src/AudioRecordWidget.h \
            $$PWD/src/Audio.h \
            $$PWD/src/wavelets/DiscretePeriodicWavelet.h \
            $$PWD/src/DenoisingManager.h

FORMS    += $$PWD/src/mainwindow.ui \
            $$PWD/src/AudioRecordWidget.ui

LIBS     += $$PWD/libs/libAquila.a\
            $$PWD/libs/libfftw3-3.dll\
            $$PWD/libs/wavelet2d.dll

CONFIG( debug, debug|release ) {
    # debug
    LIBS += $$PWD/libs/qcustomplotd1.dll
} else {
    # release
    LIBS += $$PWD/libs/qcustomplot1.dll
}

INCLUDEPATH += $$PWD/libs
DEPENDPATH  += $$PWD/libs
