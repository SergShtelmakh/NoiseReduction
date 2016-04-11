#include "PlotManager.h"

PlotManager::PlotManager(QObject *parent)
    : QObject(parent)
    , m_plot(new QCustomPlot)
    , m_minX(0)
    , m_maxX(1)
{
}

void PlotManager::setData(const QVector<double> &data)
{
    m_data = data;
}

void PlotManager::plot()
{
    Q_ASSERT(m_data.size());

    QVector<double> x(m_data.size());
    auto dx = (m_maxX - m_minX) / m_data.size();
    auto currentX = m_minX;
    auto minY = m_data.first();
    auto maxY = m_data.first();
    for (auto i = 0; i < m_data.size(); ++i) {
        x[i] = currentX;
        currentX += dx;
        minY = qMin(m_data[i], minY);
        maxY = qMax(m_data[i], maxY);
    }

    m_plot->addGraph();
    m_plot->graph(0)->setData(x, m_data);

    m_plot->xAxis->setLabel("Time");
    m_plot->yAxis->setLabel("Amplitude");

    m_plot->xAxis->setRange(m_minX, m_maxX);
    m_plot->yAxis->setRange(minY, maxY);
    m_plot->replot();
    m_plot->show();
}

double PlotManager::minX() const
{
    return m_minX;
}

void PlotManager::setMinX(double minX)
{
    m_minX = minX;
}

double PlotManager::maxX() const
{
    return m_maxX;
}

void PlotManager::setMaxX(double maxX)
{
    m_maxX = maxX;
}
