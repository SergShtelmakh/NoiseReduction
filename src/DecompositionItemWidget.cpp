#include "DecompositionItemWidget.h"
#include "ui_DecompositionItemWidget.h"

#include <qcustomplot/qcustomplot.h>

DecompositionItemWidget::DecompositionItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecompositionItemWidget)
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

DecompositionItemWidget::~DecompositionItemWidget()
{
    delete ui;
}

Audio::Signal DecompositionItemWidget::signal() const
{
    return m_signal;
}

void DecompositionItemWidget::setSignal(const Audio::Signal &signal)
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

double DecompositionItemWidget::maxThreshold() const
{
    return m_maxThreshold;
}

double DecompositionItemWidget::threshold() const
{
    return m_threshold;
}

void DecompositionItemWidget::setThreshold(double threshold)
{
    if (m_threshold == threshold) {
        return;
    }

    m_threshold = threshold;
    emit thresholdChanged(m_threshold);

    replotThreshold();
}

Audio::Signal DecompositionItemWidget::thresholded() const
{
    auto max = m_maxThreshold - m_threshold * m_maxThreshold / 100;
    Audio::Signal thresholded = m_signal;
    Audio::makeThreshold(thresholded, max);
    return thresholded;
}

void DecompositionItemWidget::updatePlotData()
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

void DecompositionItemWidget::replotSignal()
{
    ui->wPlot->graph(0)->setData(m_x, m_signal);
    ui->wPlot->replot();
}

void DecompositionItemWidget::replotThreshold()
{
    QVector<double> threshold;
    threshold.fill(m_maxThreshold - m_threshold * m_maxThreshold / 100, m_x.size());
    ui->wPlot->graph(1)->setData(m_x, threshold);

    threshold.fill(-m_maxThreshold + m_threshold * m_maxThreshold / 100, m_x.size());
    ui->wPlot->graph(2)->setData(m_x, threshold);
    ui->wPlot->replot();
}

void DecompositionItemWidget::setMaxThreshold(double maxThreshold)
{
    if (m_maxThreshold == maxThreshold) {
        return;
    }

    m_maxThreshold = maxThreshold;
    emit maxThresholdChanged(m_maxThreshold);
}

