#pragma once

#include <QWidget>
#include <QScopedPointer>

#include <src/audio/AudioSignal.h>
#include <src/SignalAnalyzer.h>
#include <src/wavelets/DiscretePeriodicWavelet.h>

namespace Ui { class AnalyzerWidget; }

class AnalyzerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnalyzerWidget(QWidget *parent = 0);
    ~AnalyzerWidget();

    void setSignal(const AudioSignal &signal);

private slots:
    void on_pbProcess_clicked();
    void log(const QString &str);
    void onOptimalThresholdsFounded(const QString &func, const QVector<double> &thresholds);

    void on_cbWaveletType_currentIndexChanged(int index);

private:
    void clearLog();

    Ui::AnalyzerWidget *ui;

    QScopedPointer<AudioSignal> m_inputSignal;
    QScopedPointer<AudioSignal> m_noisedSignal;
    QScopedPointer<AudioSignal> m_outputSignal;

    SignalAnalyzer *m_analyzer;
    QThread *m_pAnalyzerThread;
    DiscretePeriodicWavelet m_wavelet;
};
