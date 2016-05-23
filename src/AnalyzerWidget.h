#pragma once

#include <QWidget>
#include <QScopedPointer>
#include <QTimer>

#include <src/audio/AudioSignal.h>
#include <src/SignalAnalyzer.h>
#include <src/wavelets/DiscretePeriodicWavelet.h>
#include <src/PlotManager.h>

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

    void on_pbSave_clicked();

    void on_pbLoad_clicked();

    void on_pbSaveNoised_clicked();

    void on_pbStop_clicked();

    void showNextFrame();

private:
    struct AnalyzerData {
        QString wavelet;
        QVector<double> thresholds;
    };

    void clearLog();
    void startAnimation();


    Ui::AnalyzerWidget *ui;

    QScopedPointer<AudioSignal> m_inputSignal;
    QScopedPointer<AudioSignal> m_noisedSignal;
    QScopedPointer<AudioSignal> m_outputSignal;

    SignalAnalyzer *m_analyzer;
    QThread *m_pAnalyzerThread;
    DiscretePeriodicWavelet m_wavelet;
    QVector<AnalyzerData> m_analyzerData;

    QCustomPlot *m_animationWidget;
    QTimer m_animationTimer;
    int m_currentFrame = -1;

    PlotManager::PlotData m_diffPlotData;

};
