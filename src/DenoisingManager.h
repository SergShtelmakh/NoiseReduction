#pragma once

#include <QObject>

#include <src/wavelets/wavelet.h>

#include <QScopedPointer>

class DenoisingManager : public QObject
{
public:
    DenoisingManager();

    void setSignal(const AudioSignal& signal);

    void makeTestNoise();
    void denoise();

    Audio::SignalStd inputSignal() const;
    Audio::SignalStd transformedSignal() const;
    Audio::SignalsVectorStd transformedDecomposition() const;
    Audio::SignalStd outputSignal() const;

private:
    Audio::SignalStd m_inputSignal;
    Audio::SignalStd m_transformedSignal;
    Audio::SignalStd m_outputSignal;

    QScopedPointer<Wavelet> m_wavelet;
};
