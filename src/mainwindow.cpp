#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <src/audio/AudioSignal.h>
#include <src/audio/AudioRecordWidget.h>
#include <src/DenoisingWidget.h>

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
    , m_signal(new AudioSignal(cTestFile))
    , m_recordWidget(new AudioRecordWidget)
    , m_denoisingWidget(new DenoisingWidget)
{
    ui->setupUi(this);
    ui->audioPlayerWidget->setSignalSource(m_signal->source());
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

}

void MainWindow::on_pbMakeWhiteNoise_clicked()
{

}

void MainWindow::on_pbManualDenoising_clicked()
{

}

void MainWindow::on_pbAutomaticDenoising_clicked()
{

}
