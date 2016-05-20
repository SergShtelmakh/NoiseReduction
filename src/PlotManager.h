#pragma once

#include <src/audio/AudioSignal.h>

class QCustomPlot;

namespace PlotManager {

struct PlotData {
    QVector<double> x;
    QVector<double> y;
    double minX = 0.0;
    double maxX = 0.0;
    double minY = 0.0;
    double maxY = 0.0;

    PlotData(QVector<double> x,  QVector<double> y, double minX = 0.0, double maxX = 0.0, double minY = 0.0, double maxY = 0.0)
        : x(x), y(y), minX(minX), maxX(maxX), minY(minY), maxY(maxY) {}
    PlotData() {}
};
    PlotData createPlotData(const Audio::SignalSource &data, double minX, double maxX);

    void plot(QCustomPlot* widget, const Audio::SignalSource &data, double minX, double maxX, bool interaction = true);
    void plot(QCustomPlot *widget, const AudioSignal &signal);

    void createPlot(const Audio::SignalSource &data);
}
