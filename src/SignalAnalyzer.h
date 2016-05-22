#pragma once

#include <QObject>
#include <src/audio/AudioSignal.h>
#include <src/wavelets/DiscretePeriodicWavelet.h>

class SignalAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit SignalAnalyzer(QObject *parent = 0);

    void setData(const AudioSignal &inputSignal, const AudioSignal &noisedSignal, double maxNoiseAmplitude);

signals:
    void log(const QString& str);
    void started();
    void finished();
    void optimalThresholdsFounded(const QString &func, const QVector<double> &thresholds);

public slots:
    void start();

private:
    QVector<double> findOptimalThresholds(const Audio::SignalsSourceVector &signalVector, double posibleOverthresholdsCount);

    AudioSignal m_inputSignal;
    AudioSignal m_noisedSignal;
    AudioSignal m_outputSignal;
    DiscretePeriodicWavelet m_wavelet;
    double m_maxNoiseAmplitude = 1;
};

