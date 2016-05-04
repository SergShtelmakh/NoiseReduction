#include "DecompositionItem.h"
#include "ui_DecompositionItem.h"

#include <qcustomplot/qcustomplot.h>

namespace {

double makeThreshold(double input, double max) {
    auto sign = input > 0 ? 1 : -1;
    return qMin(input, max) * sign;
}

}

DecompositionItem::DecompositionItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecompositionItem)
{
    ui->setupUi(this);
    ui->sbThreshold->setMinimum(0);
    ui->sbThreshold->setMaximum(100);
    ui->sbThreshold->setValue(0);

    ui->wPlot->addGraph();
    ui->wPlot->addGraph();
    ui->wPlot->addGraph();
    ui->wPlot->xAxis->setLabel("Time");
    ui->wPlot->yAxis->setLabel("Amplitude");

    connect(ui->sbThreshold, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val){ setThreshold(val); });
}

DecompositionItem::~DecompositionItem()
{
    delete ui;
}

Audio::Signal DecompositionItem::signal() const
{
    return m_signal;
}

void DecompositionItem::setSignal(const Audio::Signal &signal)
{
    if (m_signal == signal) {
        return;
    }

    m_signal = signal;
    emit signalChanged(m_signal);

    setMaxThreshold(Audio::maxAmplitude(m_signal));

    updatePlotData();

    replotSignal();
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
    emit thresholdChanged(m_threshold);

    replotThreshold();
}

Audio::Signal DecompositionItem::thresholded() const
{
    auto max = m_maxThreshold - m_threshold * m_maxThreshold / 100;
    Audio::Signal thresholded;
    for (auto i : m_signal) {
        thresholded << makeThreshold(i, max);
    }

    return thresholded;
}

void DecompositionItem::updatePlotData()
{
    if (m_signal.size() == 0) {
        return;
    }

    m_x.clear();
    m_x.fill(0, m_signal.size());
    auto maxX = m_signal.size();
    auto minX = 0;
    auto dx = (maxX - minX) / m_signal.size();
    auto currentX = minX;
    auto minY = m_signal.first();
    auto maxY = m_signal.first();
    for (auto i = 0; i < m_signal.size(); ++i) {
        m_x[i] = currentX;
        currentX += dx;
        minY = qMin(m_signal[i], minY);
        maxY = qMax(m_signal[i], maxY);
    }

    ui->wPlot->xAxis->setRange(minX, maxX);
    ui->wPlot->yAxis->setRange(minY, maxY);
}

void DecompositionItem::replotSignal()
{
    ui->wPlot->graph(0)->setData(m_x, m_signal);
    ui->wPlot->replot();
}

void DecompositionItem::replotThreshold()
{
    QVector<double> threshold;
    threshold.fill(m_maxThreshold - m_threshold * m_maxThreshold / 100, m_x.size());
    ui->wPlot->graph(1)->setData(m_x, threshold);

    threshold.fill(-m_maxThreshold + m_threshold * m_maxThreshold / 100, m_x.size());
    ui->wPlot->graph(2)->setData(m_x, threshold);
    ui->wPlot->replot();
}

void DecompositionItem::setMaxThreshold(double maxThreshold)
{
    if (m_maxThreshold == maxThreshold) {
        return;
    }

    m_maxThreshold = maxThreshold;
    emit maxThresholdChanged(m_maxThreshold);
}

