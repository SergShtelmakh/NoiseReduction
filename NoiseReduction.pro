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
            $$PWD/src/audio/AudioSignal.cpp \
            $$PWD/src/wavelets/Wavelet.cpp \
            $$PWD/src/PlotManager.cpp \
            $$PWD/src/audio/AudioRecordWidget.cpp \
            $$PWD/src/audio/Audio.cpp \
            $$PWD/src/wavelets/DiscretePeriodicWavelet.cpp \
            $$PWD/src/DenoisingManager.cpp \
            $$PWD/src/DecompositionItemWidget.cpp \
            $$PWD/src/DecompositionWidget.cpp

HEADERS  += $$PWD/src/mainwindow.h \
            $$PWD/src/audio/AudioSignal.h \
            $$PWD/src/wavelets/Wavelet.h \
            $$PWD/src/PlotManager.h \
            $$PWD/src/audio/AudioRecordWidget.h \
            $$PWD/src/audio/Audio.h \
            $$PWD/src/wavelets/DiscretePeriodicWavelet.h \
            $$PWD/src/DenoisingManager.h \
            $$PWD/src/DecompositionItemWidget.h \
            $$PWD/src/DecompositionWidget.h

FORMS    += $$PWD/src/mainwindow.ui \
            $$PWD/src/audio/AudioRecordWidget.ui \
            $$PWD/src/DecompositionItemWidget.ui \
            $$PWD/src/DecompositionWidget.ui

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
