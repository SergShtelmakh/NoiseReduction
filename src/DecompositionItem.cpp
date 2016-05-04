#include "DecompositionItem.h"
#include "ui_DecompositionItem.h"

#include <qcustomplot/qcustomplot.h>

DecompositionItem::DecompositionItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecompositionItem)
{
    ui->setupUi(this);
}

DecompositionItem::~DecompositionItem()
{
    delete ui;
}

Audio::SignalQt DecompositionItem::signal() const
{
    return m_signal;
}

void DecompositionItem::setSignal(const Audio::SignalQt &signal)
{
    if (m_signal == signal) {
        return;
    }

    m_signal = signal;
    emit signalChanged(m_signal);

    if (m_signal.size() == 0) {
        return;
    }

    QVector<double> x(m_signal.size());
    auto maxX = m_signal.size();
    auto minX = 0;
    auto dx = (maxX - minX) / m_signal.size();
    auto currentX = minX;
    auto minY = m_signal.first();
    auto maxY = m_signal.first();
    for (auto i = 0; i < m_signal.size(); ++i) {
        x[i] = currentX;
        currentX += dx;
        minY = qMin(m_signal[i], minY);
        maxY = qMax(m_signal[i], maxY);
    }

    ui->wPlot->addGraph();
    ui->wPlot->graph(0)->setData(x, m_signal);

    ui->wPlot->xAxis->setLabel("Time");
    ui->wPlot->yAxis->setLabel("Amplitude");

    ui->wPlot->xAxis->setRange(minX, maxX);
    ui->wPlot->yAxis->setRange(minY, maxY);
    ui->wPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->wPlot->replot();
}

double DecompositionItem::maxThreshold() const
{
    return m_maxThreshold;
}

double DecompositionItem::threshold() const
{
    return m_threshold;
}

void DecompositionItem::setThreshold(double threshold)
{
    if (m_threshold == threshold) {
        return;
    }

    m_threshold = threshold;
    thresholdChanged(m_threshold);
}

void DecompositionItem::setMaxThreshold(double maxThreshold)
{
    if (m_maxThreshold == maxThreshold) {
        return;
    }

    m_maxThreshold = maxThreshold;
    emit maxThresholdChanged(m_maxThreshold);
}

