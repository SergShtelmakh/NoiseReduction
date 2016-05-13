#include <src/mainwindow.h>
#include "ui_mainwindow.h"

#include <src/audio/AudioSignal.h>
#include <src/DenoisingManager.h>
#include <src/audio/AudioRecordWidget.h>

#include <src/DecompositionWidget.h>

#include <QFileDialog>
#include <QDebug>
#include <QTime>

#include <QSound>

namespace {
    const QString cTestFile  = "test.wav";
    const QString cNoiseFile = "noise.wav";

//    QString outputFileName() {
//        return qApp->applicationDirPath() + QString("/res%1.wav").arg(QTime::currentTime().toString("hh_mm_ss_zzz"));
//    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_plotManager(new PlotManager)
    , m_recordWidget(new AudioRecordWidget)
    , m_testSignal(new AudioSignal)
    , m_noisedSignal(new AudioSignal)
    , m_decompositionWidget(new DecompositionWidget)
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
    m_testSignal->load(cTestFile);
    makePlot(PlotType::InputSignal, *m_testSignal.data());

    m_noisedSignal->load(cTestFile);
    makePlot(PlotType::NoiseSignal, *m_noisedSignal.data());

    m_decompositionWidget->show();

    auto wdg = new DecompositionWidget(true);
    wdg->show();
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

void MainWindow::makePlot(MainWindow::PlotType type, const AudioSignal &signal)
{
    PlotManager::plot(getWidgetForPlot(type), signal);
}

void MainWindow::on_cbWaveletType_currentIndexChanged(int)
{
//    m_signalWavelet->setWaveletFunction(static_cast<Wavelet::WaveletFunction>(index));
//    m_noiseWavelet->setWaveletFunction(static_cast<Wavelet::WaveletFunction>(index));
}

void MainWindow::on_leLevel_textChanged(const QString &)
{
//    bool ok;
//    int result = QString(arg1).toInt(&ok);
//    auto level = ok ? result : 1;
//    m_signalWavelet->setLevel(level);
//    m_noiseWavelet->setLevel(level);
}

void MainWindow::on_cbTransformType_currentIndexChanged(int)
{
}
