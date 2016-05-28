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
    ui->pbStopRecord->setEnabled(false);
}

AudioRecordWidget::~AudioRecordWidget()
{
    delete ui;
}

void AudioRecordWidget::on_pbRecord_clicked()
{
    qDebug() << "Record clicked";
    if (m_state == State::Record) {
        qDebug() << "Already recorded";
        return;
    }
    auto output = QUrl::fromLocalFile(Audio::generateAudioFileName());
    m_fileName = output.toString();
    qDebug() << "Recorded to " << m_fileName;
    m_recorder->setOutputLocation(output);
    m_state = State::Record;

    ui->pbRecord->setEnabled(false);
    ui->pbStopRecord->setEnabled(true);
    m_recorder->record();
}

void AudioRecordWidget::on_pbStopRecord_clicked()
{
    qDebug() << "Stop clicked";
    if (m_state == State::Stop) {
        qDebug() << "Already stopped";
        return;
    }
    qDebug() << "Recorder state" << m_recorder->state();
    qDebug() << "toString" << m_recorder->actualLocation().toString();
    qDebug() << "toLocalFile" << m_recorder->actualLocation().toLocalFile();
    qDebug() << "toDisplayString" << m_recorder->actualLocation().toDisplayString();
    qDebug() << "audio input" << m_recorder->audioInput();
    m_recorder->stop();
    m_state = State::Stop;
    m_signal.reset(new AudioSignal(m_recorder->actualLocation().toLocalFile()));
    ui->wAudioPlayer->setEnabled(true);
    ui->pbRecord->setEnabled(true);
    ui->pbStopRecord->setEnabled(false);
}
