#ifndef AUDIORECORDWIDGET_H
#define AUDIORECORDWIDGET_H

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
    Ui::AudioRecordWidget *ui;
    QString m_fileName;
    QScopedPointer<QAudioRecorder> m_recorder;
    QScopedPointer<QAudioOutput> m_player;
};

#endif // AUDIORECORDWIDGET_H
