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
