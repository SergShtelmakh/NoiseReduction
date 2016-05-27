#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <src/audio/AudioSignal.h>
#include <src/audio/AudioRecordWidget.h>
#include <src/DenoisingWidget.h>
#include <src/PlotManager.h>
#include <src/AnalyzerWidget.h>
#include <src/ThresholdsManager.h>

namespace {
    const QString cTestFile  = "test.wav";
    const QString cNoiseFile = "noise.wav";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_sourceSignal(new AudioSignal(cTestFile))
    , m_processedSignal(new AudioSignal(m_sourceSignal->source()))
    , m_recordWidget(new AudioRecordWidget)
    , m_denoisingWidget(new DenoisingWidget)
    , m_analyzerWidget(new AnalyzerWidget)
    , m_thresholsdTestWidget(new ThresholdTestWidget)
{
    ui->setupUi(this);
    updatePlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRecorder_triggered()
{
    m_recordWidget->show();
}

void MainWindow::on_pbShowSourceSignal_clicked()
{
    PlotManager::createPlot(m_sourceSignal->source());
}

void MainWindow::on_pbMakeWhiteNoise_clicked()
{
    m_processedSignal->makeWhiteNoise();
    updatePlot();
}

void MainWindow::on_pbManualDenoising_clicked()
{
    m_denoisingWidget->setSignal(*m_processedSignal.data());
    m_denoisingWidget->show();
}

void MainWindow::on_pbAutomaticDenoising_clicked()
{
    const auto level = 5;
    const auto wavelet = Wavelet::Daubechies2;
    const auto neighborhufCount = 30;

    DiscretePeriodicWavelet wave;
    wave.setSignal(m_processedSignal->source());
    wave.setLevel(level);
    wave.setWaveletFunction(wavelet);
    wave.makeTransform();

    auto vectorSignal = wave.transformedSignalVector();
    Audio::SignalsSourceVector vect;
    for (auto i : vectorSignal) {
        //auto t = Audio::overThresholdsAmplitudeSum(i, 0, neighborhufCount);
        auto t2 = Audio::makeSignalDensity(i, true);
        double tr = Audio::findDerivative(t2, -2, 100);
        vect << ThresholdsManager::threshodedSignal(ThresholdsManager::Fuzzy, i, tr /*/ neighborhufCount*/);
    }

    wave.setTransformedSignalVector(vect);
    wave.makeInverseTransform();
    m_processedSignal.reset(new AudioSignal(wave.outputSignal()));
    updatePlot();

}

void MainWindow::updatePlot()
{
    ui->audioPlayerWidget->setSignal(*m_processedSignal.data());
}

void MainWindow::on_actionOpen_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Audio"), "", tr("Wave Files (*.wav)"));
    if (QFileInfo(fileName).exists()) {
        m_sourceSignal.reset(new AudioSignal(fileName));
        m_processedSignal.reset(new AudioSignal(m_sourceSignal->source()));
        updatePlot();
    }
}

void MainWindow::on_actionSave_triggered()
{
    m_processedSignal->save(QFileDialog::getSaveFileName(this, tr("Save Audio"), "", tr("Wave Files (*.wav)")));
}

void MainWindow::on_pbRevert_clicked()
{
    m_processedSignal.reset(new AudioSignal(m_sourceSignal->source()));
    updatePlot();
}

void MainWindow::on_pbThresholdsTest_clicked()
{
    m_thresholsdTestWidget->setSignal(*m_processedSignal.data());
    m_thresholsdTestWidget->show();
}

void MainWindow::on_pushButton_clicked()
{
    m_analyzerWidget->setSignal(*m_processedSignal.data());
    m_analyzerWidget->show();
}
