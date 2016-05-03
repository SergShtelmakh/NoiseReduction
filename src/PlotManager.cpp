#include "PlotManager.h"

#include <qcustomplot/qcustomplot.h>

PlotManager::PlotManager(QObject *parent)
    : QObject(parent)
{
}

void PlotManager::plot(QCustomPlot *widget, const Audio::qtSignal &data, double minX, double maxX)
{
    if (!widget || data.size() == 0) {
        return;
    }

    QVector<double> x(data.size());
    auto dx = (maxX - minX) / data.size();
    auto currentX = minX;
    auto minY = data.first();
    auto maxY = data.first();
    for (auto i = 0; i < data.size(); ++i) {
        x[i] = currentX;
        currentX += dx;
        minY = qMin(data[i], minY);
        maxY = qMax(data[i], maxY);
    }

    widget->addGraph();
    widget->graph(0)->setData(x, data);

    widget->xAxis->setLabel("Time");
    widget->yAxis->setLabel("Amplitude");

    widget->xAxis->setRange(minX, maxX);
    widget->yAxis->setRange(minY, maxY);
    widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    widget->replot();
}

void PlotManager::plot(QCustomPlot *widget, const Audio::stdSignal &data, double minX, double maxX)
{
    plot(widget, Audio::qtSignal::fromStdVector(data), minX, maxX);
}

void PlotManager::plot(QCustomPlot *widget, const AudioSignal &signal)
{
    plot(widget, signal.qtSignal(), 0, signal.audioLength());
}

void PlotManager::createDecompositionPlot(const Audio::qtSignalsVector &data)
{
    if (data.empty()) {
        return;
    }

    auto decompositionPlot = new QWidget();
    auto mainLayout = new QVBoxLayout(decompositionPlot);

    for (auto signal : data) {
        auto currentPlot = new QCustomPlot(decompositionPlot);
        plot(currentPlot, signal, 0, signal.size());
        mainLayout->addWidget(currentPlot);
    }
    decompositionPlot->setGeometry(100, 100, 500, 500);
    decompositionPlot->setLayout(mainLayout);
    decompositionPlot->show();
}

void PlotManager::createDecompositionPlot(const Audio::stdSignalsVector &data)
{
    Audio::qtSignalsVector vector;
    for (auto i : data) {
        vector << Audio::qtSignal::fromStdVector(i);
    }
    createDecompositionPlot(vector);
}
