#include "ThresholdsWidget.h"
#include "ui_ThresholdsWidget.h"

#include <qcustomplot/qcustomplot.h>

namespace {
    const QPen cThresholdLevelPen = QPen(QBrush(Qt::red), 1);
}

ThresholdsWidget::ThresholdsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThresholdsWidget)
{
    ui->setupUi(this);

    ui->wPlot->addGraph();
    ui->wPlot->addGraph();
    ui->wPlot->addGraph();
    ui->wPlot->graph(1)->setPen(cThresholdLevelPen);
    ui->wPlot->graph(2)->setPen(cThresholdLevelPen);
    ui->wPlot->xAxis->setLabel("Time");
    ui->wPlot->yAxis->setLabel("Amplitude");
}

ThresholdsWidget::~ThresholdsWidget()
{
    delete ui;
}

Audio::SignalSource ThresholdsWidget::signalSource() const
{
    return m_signalSource;
}

void ThresholdsWidget::setSignalSource(const Audio::SignalSource &signal)
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

double ThresholdsWidget::maxThreshold() const
{
    return m_maxThreshold;
}

double ThresholdsWidget::threshold() const
{
    return m_threshold;
}

void ThresholdsWidget::setThreshold(double threshold)
{
    if (m_threshold == threshold) {
        return;
    }

    m_threshold = threshold;

    ui->vsThresholdsLevel->setValue(static_cast<int>(threshold));
    ui->sbThresholdsLevel->setValue(static_cast<int>(threshold));

    emit thresholdChanged(m_threshold);

    replotThreshold();
}

void ThresholdsWidget::updatePlotData()
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

void ThresholdsWidget::replotSignal()
{
    ui->wPlot->graph(0)->setData(m_x, m_signalSource);
    ui->wPlot->replot();
}

void ThresholdsWidget::replotThreshold()
{
    QVector<double> thresholdVector;

    thresholdVector.fill(m_threshold, m_x.size());
    ui->wPlot->graph(1)->setData(m_x, thresholdVector);
    thresholdVector.fill(-m_threshold, m_x.size());
    ui->wPlot->graph(2)->setData(m_x, thresholdVector);

    ui->wPlot->replot();
}

void ThresholdsWidget::setMaxThreshold(double maxThreshold)
{
    if (m_maxThreshold == maxThreshold) {
        return;
    }

    m_maxThreshold = maxThreshold;

    ui->sbThresholdsLevel->setMaximum(static_cast<int>(maxThreshold));
    ui->sbThresholdsLevel->setMinimum(0);
    ui->sbThresholdsLevel->setValue(0);

    ui->vsThresholdsLevel->setMaximum(static_cast<int>(maxThreshold));
    ui->vsThresholdsLevel->setMinimum(0);
    ui->vsThresholdsLevel->setValue(0);

    emit maxThresholdChanged(m_maxThreshold);
}


void ThresholdsWidget::on_vsThresholdsLevel_valueChanged(int value)
{
    setThreshold(value);
}

void ThresholdsWidget::on_sbThresholdsLevel_valueChanged(int arg1)
{
    setThreshold(arg1);
}
