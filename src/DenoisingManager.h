#pragma once

#include <QObject>

#include <src/wavelets/DiscretePeriodicWavelet.h>
#include <src/audio/AudioSignal.h>
#include <src/ThresholdsManager.h>

#include <QScopedPointer>

class DenoisingManager : public QObject
{
public:
    DenoisingManager();

    void setSignal(const AudioSignal& signal);
    void prepareToDenoising(const QString& waveletName, int level);
    void denoising(const QString& thresholdType, const QVector<double>& thresholds);

    Audio::SignalSource inputSignal() const { return m_inputSignal; }
    Audio::SignalSource transformedSignal() const { return m_transformedSignal; }
    Audio::SignalSource outputSignal() const { return m_outputSignal; }

    Audio::SignalsSourceVector transformedDecomposition() const;
    Audio::SignalSource thresholdedSignal() const;

private:
    Audio::SignalSource m_inputSignal;
    Audio::SignalSource m_transformedSignal;
    Audio::SignalSource m_outputSignal;

    QScopedPointer<DiscretePeriodicWavelet> m_wavelet;
    QScopedPointer<ThresholdsManager> m_thresholdsManager;
};
