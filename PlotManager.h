#ifndef PLOTMANAGER_H
#define PLOTMANAGER_H

#include <QObject>

#include <qcustomplot/qcustomplot.h>

class PlotManager : public QObject
{
    Q_OBJECT
public:
    explicit PlotManager(QObject *parent = 0);

    void setData(const QVector<double> &data);
    void plot();

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

#endif // PLOTMANAGER_H
