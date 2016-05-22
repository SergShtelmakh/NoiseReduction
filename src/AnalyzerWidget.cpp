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
{
    ui->setupUi(this);

    connect(m_analyzer, &SignalAnalyzer::log, this, &AnalyzerWidget::log);
    connect(m_pAnalyzerThread, &QThread::started, m_analyzer, &SignalAnalyzer::start);
    connect(m_analyzer, &SignalAnalyzer::finished, m_pAnalyzerThread, &QThread::quit);
    connect(m_analyzer, &SignalAnalyzer::optimalThresholdsFounded, this, &AnalyzerWidget::onOptimalThresholdsFounded);

    m_analyzer->moveToThread(m_pAnalyzerThread);
}

AnalyzerWidget::~AnalyzerWidget()
{
    m_analyzer->deleteLater();
    delete ui;
}

void AnalyzerWidget::setSignal(const AudioSignal &signal)
{
    m_inputSignal.reset(new AudioSignal(signal.source()));
    ui->wInputSignalPlayer->setSignalSource(m_inputSignal->source());
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
    m_analyzer->setData(*m_noisedSignal.data(), ui->sbMaxNoiseAmplitude->value());
    ui->wNoisedIputSignalPlayer->setSignalSource(m_noisedSignal->source());
    PlotManager::plot(ui->wInputDifferencePlot, Audio::makeSignalDifference(m_inputSignal->source(), m_noisedSignal->source()));

    m_pAnalyzerThread->start();

    m_wavelet.setSignal(m_noisedSignal->source());
    m_wavelet.setLevel(cLevel);
}

void AnalyzerWidget::clearLog()
{
    ui->teLog->clear();
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
            manager.setThresholdType(ThresholdsManager::Soft);
            manager.makeThreshold(vectorData);
            m_wavelet.setTransformedSignalVector(manager.thresholdedSignalsVector());
            m_wavelet.makeInverseTransform();
            m_outputSignal.reset(new AudioSignal(m_wavelet.outputSignal()));
            ui->wOutputSignalPlayer->setSignalSource(m_outputSignal->source());
            PlotManager::plot(ui->wOutputDifferencePlot, Audio::makeSignalDifference(m_noisedSignal->source(), m_outputSignal->source()));
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

    auto inputFileName = qApp->applicationDirPath() + "/" + Audio::generateAudioFileName("input");
    m_inputSignal->save(inputFileName);
    out << inputFileName << "\n";

    auto noisedFileName = qApp->applicationDirPath() + "/"  + Audio::generateAudioFileName("noised");
    m_noisedSignal->save(noisedFileName);
    out << noisedFileName << "\n";

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

    auto inputFileName = out.readLine();
    m_inputSignal.reset(new AudioSignal(inputFileName));
    ui->wInputSignalPlayer->setSignalSource(m_inputSignal->source());

    auto noisedFileName = out.readLine();
    m_noisedSignal.reset(new AudioSignal(noisedFileName));
    ui->wNoisedIputSignalPlayer->setSignalSource(m_noisedSignal->source());
    PlotManager::plot(ui->wInputDifferencePlot, Audio::makeSignalDifference(m_inputSignal->source(), m_noisedSignal->source()));

    auto dataSize = out.readLine().toLongLong();
    for (int64_t i = 0; i < dataSize; i++) {
        auto wavelet = out.readLine();
        auto thresholdsSize = out.readLine().toLongLong();
        QVector<double> thresholdVector;
        for (int64_t j = 0; j < thresholdsSize; j++) {
            thresholdVector << out.readLine().toDouble();
        }

        QVariant data;
        data.setValue<QVector<double>>(thresholdVector);
        ui->cbWaveletType->addItem(wavelet, data);
        m_analyzerData.append({ wavelet , thresholdVector });
    }
}
