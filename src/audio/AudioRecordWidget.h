#pragma once

#include <QWidget>
#include <src/audio/AudioSignal.h>

namespace Ui { class AudioRecordWidget; }

class QAudioRecorder;
class QAudioOutput;

class AudioRecordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AudioRecordWidget(QWidget *parent = 0);
    ~AudioRecordWidget();

private slots:
    void on_pbRecord_clicked();

    void on_pbStopRecord_clicked();

private:
    enum class State {
        Record,
        Play,
        Stop,
        Invalid = -1
    };

    Ui::AudioRecordWidget *ui;
    QString m_fileName;
    QScopedPointer<QAudioRecorder> m_recorder;
    QScopedPointer<QAudioOutput> m_player;
    State m_state = State::Stop;
    QScopedPointer<AudioSignal> m_signal;
};
