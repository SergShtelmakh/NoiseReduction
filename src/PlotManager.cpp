#include "PlotManager.h"

#include <qcustomplot/qcustomplot.h>

PlotManager::PlotManager(QObject *parent)
    : QObject(parent)
{
}

void PlotManager::makePlot(QCustomPlot *widget, const QVector<double> &data, double minX, double maxX)
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
