#pragma once

#include <QWidget>

namespace Ui {
class AudioRecordWidget;
}

class QAudioRecorder;
class QAudioOutput;

class AudioRecordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudioRecordWidget(QWidget *parent = 0);
    ~AudioRecordWidget();

private slots:
    void on_pbBrowse_clicked();
    void on_pbRecord_clicked();
    void on_pbPlay_clicked();
    void on_pbStop_clicked();

    void on_leFilePath_textChanged(const QString &arg1);

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
    State m_state;
};
