#include "AudioPlayerWidget.h"
#include "ui_AudioPlayerWidget.h"

AudioPlayerWidget::AudioPlayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioPlayerWidget)
{
    ui->setupUi(this);
}

AudioPlayerWidget::~AudioPlayerWidget()
{
    delete ui;
}

void AudioPlayerWidget::on_pbPlay_clicked()
{

}

void AudioPlayerWidget::on_pbPause_clicked()
{

}

void AudioPlayerWidget::on_pbStop_clicked()
{

}
