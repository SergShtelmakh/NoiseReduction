#include "DecompositionItemWidget.h"
#include "ui_DecompositionItemWidget.h"

#include <qcustomplot/qcustomplot.h>

namespace {
    const QPen cThresholdLevelPen = QPen(QBrush(Qt::red), 1);
}

DecompositionItemWidget::DecompositionItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecompositionItemWidget)
{
    ui->setupUi(this);
    ui->vsThresholdsLevel->setMinimum(0);
    ui->vsThresholdsLevel->setMaximum(100);
    ui->vsThresholdsLevel->setValue(0);

    ui->wPlot->addGraph();
    ui->wPlot->addGraph();
    ui->wPlot->addGraph();
    ui->wPlot->graph(1)->setPen(cThresholdLevelPen);
    ui->wPlot->graph(2)->setPen(cThresholdLevelPen);
    ui->wPlot->xAxis->setLabel("Time");
    ui->wPlot->yAxis->setLabel("Amplitude");
}

DecompositionItemWidget::~DecompositionItemWidget()
{
    delete ui;
}

Audio::SignalSource DecompositionItemWidget::signal() const
{
    return m_signalSource;
}

void DecompositionItemWidget::setSignalSource(const Audio::SignalSource &signal)
{
    if (m_signalSource == signal) {
        return;
    }

    m_signalSource = signal;
    emit signalChanged(m_signalSource);

    setMaxThreshold(Audio::maxAmplitude(m_signalSource));

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

double DecompositionItemWidget::thresholdValue() const
{
    return m_maxThreshold - m_threshold * m_maxThreshold / 100;
}

void DecompositionItemWidget::updatePlotData()
{
    if (m_signalSource.size() == 0) {
        return;
    }

    m_x.clear();
    m_x.fill(0, m_signalSource.size());
    auto maxX = m_signalSource.size();
    auto minX = 0;
    auto dx = (maxX - minX) / m_signalSource.size();
    auto currentX = minX;
    auto minY = m_signalSource.first();
    auto maxY = m_signalSource.first();
    for (auto i = 0; i < m_signalSource.size(); ++i) {
        m_x[i] = currentX;
        currentX += dx;
        minY = qMin(m_signalSource[i], minY);
        maxY = qMax(m_signalSource[i], maxY);
    }

    ui->wPlot->xAxis->setRange(minX, maxX);
    ui->wPlot->yAxis->setRange(minY, maxY);
}

void DecompositionItemWidget::replotSignal()
{
    ui->wPlot->graph(0)->setData(m_x, m_signalSource);
    ui->wPlot->replot();
}

void DecompositionItemWidget::replotThreshold()
{
    QVector<double> threshold;
    double threshVal = thresholdValue();

    threshold.fill(threshVal, m_x.size());
    ui->wPlot->graph(1)->setData(m_x, threshold);
    threshold.fill(-threshVal, m_x.size());
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


void DecompositionItemWidget::on_vsThresholdsLevel_valueChanged(int value)
{
    setThreshold(value);
}
