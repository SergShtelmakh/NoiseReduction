#include <src/mainwindow.h>
#include "ui_mainwindow.h"

#include <aquila/global.h>

#include <wavelet2d/wavelet2d.h>

#include <QFileDialog>
#include <QDebug>
#include <QTime>



namespace {
    const QString cTestFile  = "test.wav";
    const QString cNoiseFile = "noise.wav";

    Signal makeSignal(const Aquila::WaveFile &wave) {
        Signal result;
        for (auto sample : wave) {
            result.push_back(sample);
        }
        return result;
    }

    QVector<double> makeQVector (const Aquila::WaveFile &wave) {
        return QVector<double>::fromStdVector(makeSignal(wave));
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_plotManager(new PlotManager)
    , m_recordWidget(new AudioRecordWidget)
    , m_signalWavelet(Wavelet::create(Wavelet::WaveletTransformType::DiscretePeriodic1D))
    , m_noiseWavelet(Wavelet::create(Wavelet::WaveletTransformType::DiscretePeriodic1D))
{
    ui->setupUi(this);
    ui->cbWaveletType->addItems(Wavelet::waveletFunctionsNames());
    ui->cbTransformType->addItems(Wavelet::makeTransformsNames());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbStart_clicked()
{
    // Libs test

    //auto fileName = QFileDialog::getOpenFileName(this, tr("Open wave file"), "", tr("Wave Files (*.wav)"));

    auto testFile = Aquila::WaveFile(QString(cTestFile).toStdString());
    makePlot(PlotType::InputSignal, testFile);

    auto noiseFile = Aquila::WaveFile(QString(cNoiseFile).toStdString());
    makePlot(PlotType::NoiseSignal, noiseFile);

    makeTransform(SignalForTransform::Input, makeSignal(testFile));
    makeTransform(SignalForTransform::Noise, makeSignal(noiseFile));

    m_signalWavelet->denoising();
    m_signalWavelet->makeInverseTransform();
    makePlot(PlotType::ResultSignal, m_signalWavelet->resultSignal());

    auto fileName = qApp->applicationDirPath() + QString("/res%1.wav").arg(QTime::currentTime().toString("hh_mm_ss_zzz"));
    qDebug() << fileName;
    Aquila::WaveFile::save(Aquila::SignalSource(m_signalWavelet->resultSignal(), testFile.getSampleFrequency()), fileName.toStdString());

}

void MainWindow::on_pbStop_clicked()
{

}

void MainWindow::on_actionRecorder_triggered()
{
    m_recordWidget->show();
}

void MainWindow::log(const QString &str)
{
    ui->tbResult->append(QString("[%1] %2\n").arg(QTime::currentTime().toString(), str));
}

void MainWindow::makeTransform(MainWindow::SignalForTransform sigType, const Signal &signal)
{
    switch (sigType) {
    case SignalForTransform::Input:
        log("Prepare to input transform");
        m_signalWavelet->makeTransform(signal);
        makePlot(PlotType::InputSignalTransformed, m_signalWavelet->transformedSignal());
        break;
    case SignalForTransform::Noise:
        log("Prepare to noise transform");
        m_noiseWavelet->makeTransform(signal);
        makePlot(PlotType::NoiseSignalTransformed, m_noiseWavelet->transformedSignal());
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    log(m_signalWavelet->resultText());
    log(m_noiseWavelet->resultText());
}

QCustomPlot *MainWindow::getWidgetForPlot(MainWindow::PlotType type)
{
    switch (type) {
    case PlotType::InputSignal:
        return ui->inpSignPlot;
    case PlotType::InputSignalTransformed:
        return ui->inpSignTransPlot;
    case PlotType::NoiseSignal:
        return ui->noisePlot;
    case PlotType::NoiseSignalTransformed:
        return ui->noiseTransPlot;
    case PlotType::ResultSignal:
        return ui->resultPlot;
    default:
        Q_ASSERT(false);
        break;
    }

    return nullptr;
}

void MainWindow::makePlot(MainWindow::PlotType type, const Aquila::WaveFile &file)
{
    auto vect = makeQVector(file);
    PlotManager::makePlot(getWidgetForPlot(type), vect , 0, vect.size());
}

void MainWindow::makePlot(MainWindow::PlotType type, const Signal &signal)
{
    PlotManager::makePlot(getWidgetForPlot(type), QVector<double>::fromStdVector(signal), 0, signal.size());
}

void MainWindow::on_cbWaveletType_currentIndexChanged(int index)
{
    m_signalWavelet->setWaveletFunction(static_cast<Wavelet::WaveletFunction>(index));
    m_noiseWavelet->setWaveletFunction(static_cast<Wavelet::WaveletFunction>(index));
}

void MainWindow::on_leLevel_textChanged(const QString &arg1)
{
    bool ok;
    int result = QString(arg1).toInt(&ok);
    auto level = ok ? result : 1;
    m_signalWavelet->setLevel(level);
    m_noiseWavelet->setLevel(level);
}

void MainWindow::on_cbTransformType_currentIndexChanged(int)
{
}
