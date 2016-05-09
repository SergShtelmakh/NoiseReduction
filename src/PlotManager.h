#pragma once

#include <QObject>

#include <src/audio/Audio.h>
#include <src/audio/AudioSignal.h>

class QCustomPlot;

class PlotManager : public QObject
{
    Q_OBJECT
public:
    explicit PlotManager(QObject *parent = 0);

    static void plot(QCustomPlot* widget, const Audio::SignalSource &data, double minX, double maxX);
    static void plot(QCustomPlot* widget, const AudioSignal &signal);

    static void createDecompositionPlot(const Audio::SignalsSourceVector &data);
};
