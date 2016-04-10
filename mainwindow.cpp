#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <aquila/global.h>
#include <aquila/source/WaveFile.h>

#include <QFileDialog>
#include <QDebug>

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
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open wave file"), "", tr("Wave Files (*.wav)"));
    auto waveFile = Aquila::WaveFile(fileName.toStdString());

    qDebug() << "Loaded file:" << QString::fromStdString(waveFile.getFilename());
    qDebug() << "Sample count" << waveFile.getSamplesCount();
    for (auto sample : waveFile) {
        qDebug() << "Current sample" << sample;
    }

}

void MainWindow::on_pbStop_clicked()
{

}
