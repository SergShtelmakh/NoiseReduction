#pragma once

#include <QWidget>

#include <src/audio/Audio.h>
#include <src/PlotManager.h>

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
    void replotSignal();
    void replotThreshold();
    void replotDensity();

    Audio::SignalSource m_signalSource;
    double m_maxThreshold;
    double m_threshold;

    QVector<double> m_x;

    QVector<double> m_positiveDensity;
    QVector<double> m_negativeDensity;

    PlotManager::PlotData m_signalPlotData;
    PlotManager::PlotData m_pdPlotData;
    PlotManager::PlotData m_ndPlotData;

    Ui::ThresholdsWidget *ui;
};
