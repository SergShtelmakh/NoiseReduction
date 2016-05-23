#include "ThresholdTestWidget.h"
#include "ui_ThresholdTestWidget.h"



ThresholdTestWidget::ThresholdTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThresholdTestWidget)
{
    ui->setupUi(this);
    ui->wResultPlot->addGraph();
    ui->wResultPlot->addGraph();
}

ThresholdTestWidget::~ThresholdTestWidget()
{
    delete ui;
}

void ThresholdTestWidget::setSignal(const AudioSignal &signal)
{
    m_inputSignal = signal;
    ui->wThresholds->setSignalSource(m_inputSignal.source());
}

void ThresholdTestWidget::on_pbProcess_clicked()
{
    auto plotY = Audio::overThresholdsAmplitudeSum(m_inputSignal.source()
                                                    , ui->wThresholds->threshold()
                                                    , ui->sbNeighbourhood->value());
    m_plotData = PlotManager::createPlotData(plotY, 0, plotY.size());
    ui->wResultPlot->graph(0)->setData(m_plotData.x, m_plotData.y);
    ui->wResultPlot->xAxis->setRange(m_plotData.minX, m_plotData.maxX);
    ui->wResultPlot->xAxis->setTickLabels(false);
    ui->wResultPlot->yAxis->setRange(m_plotData.minY, m_plotData.maxY);
    ui->wResultPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->wResultPlot->replot();

    PlotManager::createPlot(Audio::makeSignalDensity(plotY, true, 1000000));
}

void ThresholdTestWidget::on_sbLevel_valueChanged(int arg1)
{
    QVector<double> thresholdVector;
    thresholdVector.fill(arg1, m_plotData.x.size());
    ui->wResultPlot->graph(1)->setData(m_plotData.x, thresholdVector);
    ui->wResultPlot->replot();
}
