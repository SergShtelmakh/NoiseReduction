#include "AnalyzerWidget.h"
#include "ui_AnalyzerWidget.h"

#include <src/PlotManager.h>
#include <src/ThresholdsManager.h>
#include <QFileDialog>
namespace {
    const int cLevel = 5;
}

AnalyzerWidget::AnalyzerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnalyzerWidget)
    , m_analyzer(new SignalAnalyzer)
    , m_pAnalyzerThread(new QThread)
    , m_animationWidget(new QCustomPlot)
{
    ui->setupUi(this);

    connect(m_analyzer, &SignalAnalyzer::log, this, &AnalyzerWidget::log);
    connect(m_pAnalyzerThread, &QThread::started, m_analyzer, &SignalAnalyzer::start);
    connect(m_analyzer, &SignalAnalyzer::finished, m_pAnalyzerThread, &QThread::quit);
    connect(m_analyzer, &SignalAnalyzer::optimalThresholdsFounded, this, &AnalyzerWidget::onOptimalThresholdsFounded);
    connect(&m_animationTimer, &QTimer::timeout, this, &AnalyzerWidget::showNextFrame);
    connect(m_analyzer, &SignalAnalyzer::mseCalculated, this, [](const QVector<double> &vec){PlotManager::createPlot(vec);});

    m_analyzer->moveToThread(m_pAnalyzerThread);
    m_animationTimer.setInterval(1000);
    m_animationWidget->addGraph();
    ui->wInputDifferencePlot->addGraph();
    ui->wOutputDifferencePlot->addGraph();
}

AnalyzerWidget::~AnalyzerWidget()
{
    m_analyzer->deleteLater();
    delete ui;
}

void AnalyzerWidget::setSignal(const AudioSignal &signal)
{
    m_inputSignal.reset(new AudioSignal(signal.source()));
    ui->wInputSignalPlayer->setSignal(*m_inputSignal.data());
}

void AnalyzerWidget::on_pbProcess_clicked()
{
    if (m_pAnalyzerThread->isRunning()) {
        m_pAnalyzerThread->quit();
    }

    clearLog();
    ui->cbWaveletType->clear();

    m_noisedSignal.reset(new AudioSignal(m_inputSignal->source()));
    m_noisedSignal->makeWhiteNoise(ui->sbMaxNoiseAmplitude->value(), ui->dsbNoiseDensity->value());
    m_analyzer->setData(*m_inputSignal.data(), *m_noisedSignal.data(), ui->sbMaxNoiseAmplitude->value());
    ui->wNoisedIputSignalPlayer->setSignal(*m_noisedSignal.data());

    auto diff = Audio::makeSignalDifference(m_inputSignal->source(), m_noisedSignal->source());
    m_diffPlotData = PlotManager::createPlotData(diff, 0, diff.size());
    ui->wInputDifferencePlot->graph(0)->setData(m_diffPlotData.x, m_diffPlotData.y);
    ui->wInputDifferencePlot->xAxis->setRange(m_diffPlotData.minX, m_diffPlotData.maxX);
    ui->wInputDifferencePlot->yAxis->setRange(m_diffPlotData.minY, m_diffPlotData.maxY);
    ui->wInputDifferencePlot->replot();


    m_pAnalyzerThread->start();

    m_wavelet.setSignal(m_noisedSignal->source());
    m_wavelet.setLevel(cLevel);
}

void AnalyzerWidget::clearLog()
{
    ui->teLog->clear();
}

void AnalyzerWidget::startAnimation()
{
//    m_currentFrame = -1;
//    m_animationTimer.start();
//    m_animationWidget->show();
}

void AnalyzerWidget::log(const QString &str)
{
    ui->teLog->append(str);
}

void AnalyzerWidget::onOptimalThresholdsFounded(const QString &func, const QVector<double> &thresholds)
{
    QVariant data;
    data.setValue<QVector<double>>(thresholds);
    ui->cbWaveletType->addItem(func, data);
    m_analyzerData.append({ func , thresholds });
}

void AnalyzerWidget::on_cbWaveletType_currentIndexChanged(int index)
{
    if (m_pAnalyzerThread->isRunning()) {
        return;
    }
    ui->teThresholdsInfo->clear();
    if (index >= 0) {
        auto data = ui->cbWaveletType->itemData(index);
        if (data.isValid()) {
            auto vectorData = data.value<QVector<double>>();
            for (auto i = 0; i < vectorData.size(); i++) {
                ui->teThresholdsInfo->append(QString("Level %1:  threshold %2").arg(i).arg(vectorData[i]));
            }
            m_wavelet.setWaveletFunction(Wavelet::fromString(ui->cbWaveletType->currentText()));
            m_wavelet.makeTransform();

            ThresholdsManager manager;
            manager.setSignalsVector(m_wavelet.transformedSignalVector());
            manager.setThresholdType(ThresholdsManager::Fuzzy);
            manager.makeThreshold(vectorData);
            m_wavelet.setTransformedSignalVector(manager.thresholdedSignalsVector());
            m_wavelet.makeInverseTransform();
            m_outputSignal.reset(new AudioSignal(m_wavelet.outputSignal()));
            ui->wOutputSignalPlayer->setSignal(m_outputSignal->source());

            ui->wOutputDifferencePlot->graph(0)->setData(m_diffPlotData.x, Audio::makeSignalDifference(m_noisedSignal->source(), m_outputSignal->source()));
            ui->wOutputDifferencePlot->xAxis->setRange(m_diffPlotData.minX, m_diffPlotData.maxX);
            ui->wOutputDifferencePlot->yAxis->setRange(m_diffPlotData.minY, m_diffPlotData.maxY);
            ui->wOutputDifferencePlot->replot();
        }
    }
}

void AnalyzerWidget::on_pbSave_clicked()
{
    auto fileName = QFileDialog::getSaveFileName(this, tr("Save Analyzer data"), "", tr("Analyzer data (*.adat)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);

    out << m_inputSignal->source().size() << "\n";
    for (auto i : m_inputSignal->source()) {
        out << i << "\n";
    }

    out << m_noisedSignal->source().size() << "\n";
    for (auto i : m_noisedSignal->source()) {
        out << i << "\n";
    }

    out << m_analyzerData.size() << "\n";
    for (AnalyzerData i : m_analyzerData) {
        out << i.wavelet << "\n";
        out << i.thresholds.size() << "\n";
        for (auto j : i.thresholds) {
            out << j << "\n";
        }
    }
}

void AnalyzerWidget::on_pbLoad_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Analyzer data"), "", tr("Analyzer data (*.adat)"));
    if (!QFileInfo(fileName).exists()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);

    ui->cbWaveletType->clear();
    m_analyzerData.clear();

    Audio::SignalSource input;
    auto inputFileSize = out.readLine().toInt();
    for (int i = 0; i < inputFileSize; i++) {
        input << out.readLine().toDouble();
    }
    m_inputSignal.reset(new AudioSignal(input));
    ui->wInputSignalPlayer->setSignal(m_inputSignal->source());

    Audio::SignalSource noised;
    auto noisedFileSize = out.readLine().toInt();
    for (int i = 0; i < noisedFileSize; i++) {
        noised << out.readLine().toDouble();
    }
    m_noisedSignal.reset(new AudioSignal(noised));
    ui->wNoisedIputSignalPlayer->setSignal(m_noisedSignal->source());

    auto diff = Audio::makeSignalDifference(m_inputSignal->source(), m_noisedSignal->source());
    m_diffPlotData = PlotManager::createPlotData(diff, 0, diff.size());
    ui->wInputDifferencePlot->graph(0)->setData(m_diffPlotData.x, m_diffPlotData.y);
    ui->wInputDifferencePlot->xAxis->setRange(m_diffPlotData.minX, m_diffPlotData.maxX);
    ui->wInputDifferencePlot->yAxis->setRange(m_diffPlotData.minY, m_diffPlotData.maxY);
    ui->wInputDifferencePlot->replot();

    m_wavelet.setSignal(m_noisedSignal->source());
    m_wavelet.setLevel(cLevel);

    auto dataSize = out.readLine().toInt();
    for (int64_t i = 0; i < dataSize; i++) {
        auto wavelet = out.readLine();
        auto thresholdsSize = out.readLine().toInt();
        QVector<double> thresholdVector;
        for (int64_t j = 0; j < thresholdsSize; j++) {
            thresholdVector << out.readLine().toDouble();
        }

        QVariant data;
        data.setValue<QVector<double>>(thresholdVector);
        ui->cbWaveletType->addItem(wavelet, data);
        m_analyzerData.append({ wavelet , thresholdVector });
    }

    startAnimation();
}

void AnalyzerWidget::on_pbSaveNoised_clicked()
{
    m_noisedSignal->save(QFileDialog::getSaveFileName(this, tr("Save Audio signal"), "", tr("Wave (*.wav)")));
}

void AnalyzerWidget::on_pbStop_clicked()
{
    m_analyzer->stop();
}

void AnalyzerWidget::showNextFrame()
{
    m_currentFrame++;
    if (m_currentFrame > 0 && m_currentFrame < m_analyzerData.size()) {
        m_wavelet.setWaveletFunction(Wavelet::fromString(m_analyzerData[m_currentFrame].wavelet));
        m_wavelet.makeTransform();

        ThresholdsManager manager;
        manager.setSignalsVector(m_wavelet.transformedSignalVector());
        manager.setThresholdType(ThresholdsManager::Fuzzy);
        manager.makeThreshold(m_analyzerData[m_currentFrame].thresholds);
        m_wavelet.setTransformedSignalVector(manager.thresholdedSignalsVector());
        m_wavelet.makeInverseTransform();
        m_outputSignal.reset(new AudioSignal(m_wavelet.outputSignal()));
        PlotManager::plot(m_animationWidget, m_outputSignal->source());

        m_animationWidget->xAxis2->setLabel(m_analyzerData[m_currentFrame].wavelet);
        m_animationWidget->xAxis2->setLabelFont(QFont("Times", 24, QFont::Bold));
        m_animationWidget->replot();
        m_animationWidget->setGeometry(100,100,500,500);
    }
    if (!m_animationWidget->isVisible()) {
        m_animationTimer.stop();
    }
}
