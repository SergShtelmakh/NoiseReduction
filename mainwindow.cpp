#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <aquila/global.h>
#include <aquila/source/WaveFile.h>

#include <wavelet2d/wavelet2d.h>

#include <QFileDialog>
#include <QDebug>
#include <QTime>

using Signal = std::vector<Aquila::SampleType>;

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
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_plotManager(new PlotManager)
    , m_recordWidget(new AudioRecordWidget)
    , m_wavelet(new Wavelet)
{
    ui->setupUi(this);
    ui->cbWaveletType->addItems(Wavelet::makeNames());
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
    auto testSignal = makeSignal(testFile);
    PlotManager::makePlot(ui->inpSignPlot, QVector<double>::fromStdVector(testSignal), 0, testFile.getAudioLength());

    m_wavelet->makeTransform(testSignal);

    log("Test signal");
    log(m_wavelet->resultText());

    PlotManager::makePlot(ui->inpSignTransPlot, QVector<double>::fromStdVector(m_wavelet->transform()), 0, testFile.getAudioLength());

    auto noiseFile = Aquila::WaveFile(QString(cNoiseFile).toStdString());
    auto noiseSignal = makeSignal(noiseFile);
    PlotManager::makePlot(ui->noisePlot, QVector<double>::fromStdVector(noiseSignal), 0, noiseFile.getAudioLength());

    m_wavelet->makeTransform(noiseSignal);

    log("Noise signal");
    log(m_wavelet->resultText());

    PlotManager::makePlot(ui->noiseTransPlot, QVector<double>::fromStdVector(m_wavelet->transform()), 0, noiseFile.getAudioLength());

//    ui->inpSignPlot = plot3->getPlot();

//    Aquila::WaveFile::save(Aquila::SignalSource(result), "result.wav");

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

void MainWindow::on_cbWaveletType_currentIndexChanged(int index)
{
    m_wavelet->setWaveletType(static_cast<Wavelet::WaveletType>(index));
}

void MainWindow::on_leLevel_textChanged(const QString &arg1)
{
    bool ok;
    int result = QString(arg1).toInt(&ok);
    m_wavelet->setLevel(ok ? result : 1);
}
