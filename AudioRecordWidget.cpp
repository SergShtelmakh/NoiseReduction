#include "AudioRecordWidget.h"
#include "ui_AudioRecordWidget.h"

#include <QFileDialog>
#include <QAudioRecorder>
#include <QAudioOutput>

AudioRecordWidget::AudioRecordWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AudioRecordWidget)
    , m_recorder(new QAudioRecorder)
    , m_player(new QAudioOutput)
{
    ui->setupUi(this);

    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/pcm");
    audioSettings.setQuality(QMultimedia::HighQuality);

    m_recorder->setEncodingSettings(audioSettings);
}

AudioRecordWidget::~AudioRecordWidget()
{
    delete ui;
}

void AudioRecordWidget::on_pbBrowse_clicked()
{
    ui->leFilePath->setText(QFileDialog::getSaveFileName(this, tr("Save recorded file"), "", tr("Wave Files (*.wav)")));
}

void AudioRecordWidget::on_pbRecord_clicked()
{
    m_recorder->record();
}

void AudioRecordWidget::on_pbPlay_clicked()
{
    QFile sourceFile;
    sourceFile.setFileName(m_fileName);
    sourceFile.open(QIODevice::ReadOnly);

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    m_player.reset(new QAudioOutput(info, format));
    m_player->start(&sourceFile);
}

void AudioRecordWidget::on_pbStop_clicked()
{
    m_recorder->stop();
    m_player->stop();
}

void AudioRecordWidget::on_leFilePath_textChanged(const QString &arg1)
{
    m_fileName = arg1;
    m_recorder->setOutputLocation(QUrl::fromLocalFile(m_fileName));
}
