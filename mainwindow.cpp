#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <aquila/global.h>
#include <aquila/source/WaveFile.h>

#include <wavelet2d/wavelet2d.h>

#include <qcustomplot/qcustomplot.h>

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

    QCustomPlot *customPlot = new QCustomPlot();

    int size = 1000;

    QVector<double> x(size), y(size);
    for (int i=0; i<size; ++i)
    {
      x[i] = static_cast<double>(i)/size;
      y[i] = original[i*10];
      qDebug() << "X:" << x[i] << "  Y:" << y[i];
    }

    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);

    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    customPlot->xAxis->setRange(0, 1);
    customPlot->yAxis->setRange(-200, 200);
    customPlot->replot();
    customPlot->show();
}

void MainWindow::on_pbStop_clicked()
{

}
