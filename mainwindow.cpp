#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <aquila/global.h>
#include <aquila/source/WaveFile.h>

#include <wavelet2d/wavelet2d.h>

#include <QFileDialog>
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbStart_clicked()
{
    // Libs test

    auto fileName = QFileDialog::getOpenFileName(this, tr("Open wave file"), "", tr("Wave Files (*.wav)"));
    auto waveFile = Aquila::WaveFile(fileName.toStdString());

    qDebug() << "Loaded file:" << QString::fromStdString(waveFile.getFilename());
    qDebug() << "Sample count" << waveFile.getSamplesCount();
    qDebug() << QTime::currentTime();
    std::vector<Aquila::SampleType> samples;
    for (auto sample : waveFile) {
        samples.push_back(sample);
    }
    qDebug() << "Done";
    qDebug() << samples.size();
    qDebug() << QTime::currentTime();

    vector<double> original;
    original = samples;
    std::string nm = "db1";
    int J = 10;

    vector<double> swt_output;
    int length;

    swt(samples, J, nm, swt_output, length);
    qDebug() << "Done";
    qDebug() << QTime::currentTime();
}

void MainWindow::on_pbStop_clicked()
{

}
