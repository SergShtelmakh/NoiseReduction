#include "AudioRecordWidget.h"
#include "ui_AudioRecordWidget.h"

#include <QFileDialog>
#include <QAudioRecorder>
#include <QAudioOutput>

#include "Audio.h"

AudioRecordWidget::AudioRecordWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AudioRecordWidget)
    , m_recorder(new QAudioRecorder)
    , m_player(new QAudioOutput)
    , m_state(State::Stop)
{
    ui->setupUi(this);

    m_recorder->setEncodingSettings(Audio::encoderSettings());
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
    m_state = State::Record;
}

void AudioRecordWidget::on_pbPlay_clicked()
{  
    QFile sourceFile;
    sourceFile.setFileName(m_fileName);
    sourceFile.open(QIODevice::ReadOnly);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    auto format = Audio::format();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    m_player.reset(new QAudioOutput(info, format));
    m_player->start(&sourceFile);
    m_state == State::Play;
}

void AudioRecordWidget::on_pbStop_clicked()
{
    if (m_state == State::Record) {
        m_recorder->stop();
    }

    if (m_state == State::Play) {
        m_player->stop();
    }

    m_state == State::Stop;
}

void AudioRecordWidget::on_leFilePath_textChanged(const QString &arg1)
{
    m_fileName = arg1;
    m_recorder->setOutputLocation(QUrl::fromLocalFile(m_fileName));
}
