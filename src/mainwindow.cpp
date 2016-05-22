#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <src/audio/AudioSignal.h>
#include <src/audio/AudioRecordWidget.h>
#include <src/DenoisingWidget.h>
#include <src/PlotManager.h>
#include <src/AnalyzerWidget.h>

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
    m_analyzerWidget->setSignal(*m_processedSignal.data());
    m_analyzerWidget->show();
}

void MainWindow::updatePlot()
{
    ui->audioPlayerWidget->setSignalSource(m_processedSignal->source());
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
