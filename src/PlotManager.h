#pragma once

#include <QObject>

#include <src/Audio.h>
#include <src/AudioSignal.h>

class QCustomPlot;

class PlotManager : public QObject
{
    Q_OBJECT
public:
    explicit PlotManager(QObject *parent = 0);

    static void plot(QCustomPlot* widget, const Audio::SignalQt &data, double minX, double maxX);
    static void plot(QCustomPlot* widget, const Audio::SignalStd &data, double minX, double maxX);
    static void plot(QCustomPlot* widget, const AudioSignal &signal);

    static void createDecompositionPlot(const Audio::SignalsVectorQt &data);
    static void createDecompositionPlot(const Audio::SignalsVectorStd &data);
};
