#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <src/audio/AudioSignal.h>
#include <src/audio/AudioRecordWidget.h>
#include <src/DenoisingWidget.h>
#include <src/PlotManager.h>

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
    PlotManager::plot(m_sourceSignal->source());
}

void MainWindow::on_pbMakeWhiteNoise_clicked()
{
    m_processedSignal->makeWhiteNoise();
    updatePlot();
}

void MainWindow::on_pbManualDenoising_clicked()
{

}

void MainWindow::on_pbAutomaticDenoising_clicked()
{

}

void MainWindow::updatePlot()
{
    ui->audioPlayerWidget->setSignalSource(m_processedSignal->source());
}
