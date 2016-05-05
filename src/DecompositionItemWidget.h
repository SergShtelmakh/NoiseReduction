#pragma once

#include <QWidget>

#include <src/audio/Audio.h>

namespace Ui {
class DecompositionItemWidget;
}

class DecompositionItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DecompositionItemWidget(QWidget *parent = 0);
    ~DecompositionItemWidget();

    Audio::SignalSource signal() const;
    void setSignalSource(const Audio::SignalSource &signal);

    double maxThreshold() const;
    void setMaxThreshold(double maxThreshold);

    double threshold() const;
    void setThreshold(double threshold);

//    Audio::SignalSource thresholded() const;

signals:
    void signalChanged(const Audio::SignalSource &signal);
    void maxThresholdChanged(double maxThreshold);
    void thresholdChanged(double threshold);

private:
    void updatePlotData();
    void replotSignal();
    void replotThreshold();

    Audio::SignalSource m_signalSource;
    double m_maxThreshold;
    double m_threshold;

    QVector<double> m_x;

    Ui::DecompositionItemWidget *ui;
};
