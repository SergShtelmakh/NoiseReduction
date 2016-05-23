#include "ThresholdsWidget.h"
#include "ui_ThresholdsWidget.h"

#include <qcustomplot/qcustomplot.h>
#include <src/PlotManager.h>

namespace {
    const QPen cThresholdLevelPen = QPen(QBrush(Qt::red), 1);
    const int64_t cAmplitudeFrequencyStep = 10;
    const int cNeighborhudCount = 30;
}

ThresholdsWidget::ThresholdsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThresholdsWidget)
{
    ui->setupUi(this);

    ui->wPlot->addGraph(); // signal
    ui->wPlot->addGraph(); // positive threshold
    ui->wPlot->addGraph(); // negative threshold
    ui->wPlot->graph(1)->setPen(cThresholdLevelPen);
    ui->wPlot->graph(2)->setPen(cThresholdLevelPen);
    ui->wPlot->xAxis->setLabel("Time");
    ui->wPlot->yAxis->setLabel("Amplitude");

    ui->positiveDensityPlot->addGraph(); // density
    ui->positiveDensityPlot->addGraph(); // threshold
    ui->positiveDensityPlot->graph(1)->setPen(cThresholdLevelPen);
    ui->positiveDensityPlot->xAxis->setLabel("Amplitude");
    ui->positiveDensityPlot->yAxis->setLabel("N");

    ui->negativeDensityPlot->addGraph(); // density
    ui->negativeDensityPlot->addGraph(); // threshold
    ui->negativeDensityPlot->graph(1)->setPen(cThresholdLevelPen);
    ui->negativeDensityPlot->xAxis->setLabel("Time");
    ui->negativeDensityPlot->yAxis->setLabel("Amplitude");
}

ThresholdsWidget::~ThresholdsWidget()
{
    delete ui;

    m_signalSource.clear();
    m_x.clear();
    m_positiveDensity.clear();
    m_negativeDensity.clear();
    m_signalPlotData.x.clear();
    m_signalPlotData.y.clear();
    m_pdPlotData.x.clear();
    m_pdPlotData.y.clear();
    m_ndPlotData.y.clear();
    m_ndPlotData.x.clear();
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
    m_negativeDensity = Audio::overThresholdsAmplitudeSum(m_signalSource, 0, cNeighborhudCount);
    m_positiveDensity = Audio::makeSignalDensity(m_negativeDensity, true);
    auto den = Audio::findDerivative(m_positiveDensity, -1, 100);
    qDebug() << den / cNeighborhudCount;
    setThreshold(den / cNeighborhudCount);
//    m_positiveDensity = Audio::makeAmplitudeFrequency(m_signalSource, true, cAmplitudeFrequencyStep);
//    m_negativeDensity = Audio::makeAmplitudeFrequency(m_signalSource, false, cAmplitudeFrequencyStep);
    replotSignal();
    replotDensity();
    replotThreshold();
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

void ThresholdsWidget::replotSignal()
{
    if (m_signalSource.size() == 0) {
        return;
    }
    m_signalPlotData = PlotManager::createPlotData(m_signalSource, 0, m_signalSource.size());

    ui->wPlot->graph(0)->setData(m_signalPlotData.x, m_signalPlotData.y);
    ui->wPlot->xAxis->setRange(m_signalPlotData.minX, m_signalPlotData.maxX);
    ui->wPlot->yAxis->setRange(m_signalPlotData.minY, m_signalPlotData.maxY);
    ui->wPlot->replot();
}

void ThresholdsWidget::replotThreshold()
{
    QVector<double> thresholdVector;
    thresholdVector.fill(m_threshold, m_signalPlotData.x.size());
    ui->wPlot->graph(1)->setData(m_signalPlotData.x, thresholdVector);
    thresholdVector.fill(-m_threshold, m_signalPlotData.x.size());
    ui->wPlot->graph(2)->setData(m_signalPlotData.x, thresholdVector);
    ui->wPlot->replot();

    QVector<double> posT_x = { m_threshold * cNeighborhudCount, m_threshold * cNeighborhudCount + 1 };
    QVector<double> posT_y = { m_pdPlotData.minY, m_pdPlotData.maxY };
    ui->positiveDensityPlot->graph(1)->setData(posT_x, posT_y);
    ui->positiveDensityPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->positiveDensityPlot->replot();

//    QVector<double> negT_x = { -m_threshold, -m_threshold + 1 };
//    QVector<double> negT_y = { m_ndPlotData.minY, m_ndPlotData.maxY };
//    ui->negativeDensityPlot->graph(1)->setData(negT_x, negT_y);
//    ui->negativeDensityPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
//    ui->negativeDensityPlot->replot();
}

void ThresholdsWidget::replotDensity()
{
    m_pdPlotData = PlotManager::createPlotData(m_positiveDensity, 0, m_positiveDensity.size());
    ui->positiveDensityPlot->graph(0)->setData(m_pdPlotData.x, m_pdPlotData.y);
    ui->positiveDensityPlot->xAxis->setRange(m_pdPlotData.minX, m_pdPlotData.maxX);
//    ui->positiveDensityPlot->xAxis->setTickLabels(false);
    ui->positiveDensityPlot->yAxis->setRange(m_pdPlotData.minY, m_pdPlotData.maxY);
    ui->positiveDensityPlot->replot();

    m_ndPlotData = PlotManager::createPlotData(m_negativeDensity, 0, m_negativeDensity.size());
    ui->negativeDensityPlot->graph(0)->setData(m_ndPlotData.x, m_ndPlotData.y);
    ui->negativeDensityPlot->xAxis->setRange(m_ndPlotData.minX, m_ndPlotData.maxX);
//    ui->negativeDensityPlot->xAxis->setTickLabels(false);
    ui->negativeDensityPlot->yAxis->setRange(m_ndPlotData.minY, m_ndPlotData.maxY);
    ui->negativeDensityPlot->replot();
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
