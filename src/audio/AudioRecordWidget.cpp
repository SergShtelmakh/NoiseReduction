#include "AudioRecordWidget.h"
#include "ui_AudioRecordWidget.h"

#include <src/audio/Audio.h>

#include <QFileDialog>
#include <QAudioRecorder>
#include <QAudioOutput>

AudioRecordWidget::AudioRecordWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AudioRecordWidget)
    , m_recorder(new QAudioRecorder)
    , m_player(new QAudioOutput)
    , m_state(State::Stop)
{
    ui->setupUi(this);

    m_recorder->setEncodingSettings(Audio::encoderSettings());
    ui->wAudioPlayer->setEnabled(false);
    ui->pbRecord->setEnabled(true);
    ui->pbStop->setEnabled(false);
}

AudioRecordWidget::~AudioRecordWidget()
{
    delete ui;
}

//void AudioRecordWidget::on_pbBrowse_clicked()
//{
//    ui->leFilePath->setText(QFileDialog::getSaveFileName(this, tr("Save recorded file"), "", tr("Wave Files (*.wav)")));
//}

void AudioRecordWidget::on_pbRecord_clicked()
{
    if (m_state == State::Record) {
        return;
    }
    m_fileName = Audio::generateAudioFileName();
    m_recorder->setOutputLocation(qApp->applicationDirPath() + "/" + m_fileName);
    m_state = State::Record;

    ui->pbRecord->setEnabled(false);
    ui->pbStop->setEnabled(true);
    m_recorder->record();
}

//void AudioRecordWidget::on_pbPlay_clicked()
//{
//    QFile sourceFile;
//    sourceFile.setFileName(m_fileName);
//    sourceFile.open(QIODevice::ReadOnly);

//    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
//    auto format = Audio::format();
//    if (!info.isFormatSupported(format)) {
//        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
//        return;
//    }

//    m_player.reset(new QAudioOutput(info, format));
//    m_player->start(&sourceFile);
//    m_state = State::Play;
//}

void AudioRecordWidget::on_pbStop_clicked()
{
    if (m_state == State::Stop) {
        return;
    }

    m_recorder->stop();
    m_state = State::Stop;
    m_signal.reset(new AudioSignal(m_fileName));
    ui->wAudioPlayer->setEnabled(true);
    ui->pbRecord->setEnabled(true);
    ui->pbStop->setEnabled(false);
}

//void AudioRecordWidget::on_leFilePath_textChanged(const QString &arg1)
//{
//    m_fileName = arg1;
//    m_recorder->setOutputLocation(QUrl::fromLocalFile(m_fileName));
//}
