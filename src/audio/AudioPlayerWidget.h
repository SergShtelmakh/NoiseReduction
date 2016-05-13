#ifndef AUDIOPLAYERWIDGET_H
#define AUDIOPLAYERWIDGET_H

#include <QWidget>

#include <QMediaPlayer>
#include <QTimer>

#include <src/audio/AudioSignal.h>

namespace Ui {
class AudioPlayerWidget;
}

class AudioPlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudioPlayerWidget(QWidget *parent = 0);
    ~AudioPlayerWidget();

    void setFileName(const QString &fileName);
    void setSignalSource(const Audio::SignalSource& signalSource);

private slots:
    void on_pbPlay_clicked();
    void on_pbPause_clicked();
    void on_pbStop_clicked();

    void timerTimeout();

private:
    struct Ranges { double minX; double maxX; double minY; double maxY; };

    void updateSignalPlot();
    void updateTimeLinePlot();

    Ui::AudioPlayerWidget *ui;

    QScopedPointer<AudioSignal> m_signal;
    double m_currentTime = 0.0;
    Ranges m_plotRanges;
    QMediaPlayer m_player;
    QTimer m_playerStatusChecker;
};

#endif // AUDIOPLAYERWIDGET_H
