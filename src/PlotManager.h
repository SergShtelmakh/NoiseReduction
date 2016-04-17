#pragma once

#include <QObject>

class QCustomPlot;

class PlotManager : public QObject
{
    Q_OBJECT
public:
    explicit PlotManager(QObject *parent = 0);

    static void makePlot(QCustomPlot* widget, const QVector<double> &data, double minX, double maxX);
};
