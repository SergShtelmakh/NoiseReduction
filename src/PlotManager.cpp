#include "PlotManager.h"

#include <qcustomplot/qcustomplot.h>

PlotManager::PlotManager(QObject *parent)
    : QObject(parent)
{
}

void PlotManager::plot(QCustomPlot *widget, const Audio::SignalSource &data, double minX, double maxX)
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

void PlotManager::plot(QCustomPlot *widget, const AudioSignal &signal)
{
    plot(widget, signal.source(), 0, signal.audioLength());
}

void PlotManager::plot(const Audio::SignalSource &data)
{
    QCustomPlot * pl = new QCustomPlot();
    plot(pl,data,0,data.size());
    pl->show();
}

void PlotManager::createDecompositionPlot(const Audio::SignalsSourceVector &data)
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

