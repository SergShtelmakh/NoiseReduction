#pragma once

#include <QWidget>

#include <src/audio/Audio.h>

namespace Ui { class ThresholdsWidget; }

class ThresholdsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ThresholdsWidget(QWidget *parent = 0);
    ~ThresholdsWidget();

    Audio::SignalSource signalSource() const;
    void setSignalSource(const Audio::SignalSource &signalSource);

    double maxThreshold() const;
    void setMaxThreshold(double maxThreshold);

    double threshold() const;
    void setThreshold(double threshold);

signals:
    void signalChanged(const Audio::SignalSource &signalSource);
    void maxThresholdChanged(double maxThreshold);
    void thresholdChanged(double threshold);

private slots:
    void on_vsThresholdsLevel_valueChanged(int value);
    void on_sbThresholdsLevel_valueChanged(int arg1);

private:
    void updatePlotData();
    void replotSignal();
    void replotThreshold();

    Audio::SignalSource m_signalSource;
    double m_maxThreshold;
    double m_threshold;

    QVector<double> m_x;

    Ui::ThresholdsWidget *ui;
};
