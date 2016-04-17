#pragma once

#include <QObject>

#include <qcustomplot/qcustomplot.h>

class PlotManager : public QObject
{
    Q_OBJECT
public:
    explicit PlotManager(QObject *parent = 0);

    static void makePlot(QCustomPlot* widget, const QVector<double> &data, double minX, double maxX);

    void setData(const QVector<double> &data);
    void plot();
    QCustomPlot *getPlot();

    double minX() const;
    void setMinX(double minX);

    double maxX() const;
    void setMaxX(double maxX);

signals:

public slots:

private:
    QVector<double> m_data;
    QScopedPointer<QCustomPlot> m_plot;
    double m_minX;
    double m_maxX;
};
