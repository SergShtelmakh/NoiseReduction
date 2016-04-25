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

    Audio::stdSignal inputSignal() const;
    Audio::stdSignal transformedSignal() const;
    Audio::stdSignal outputSignal() const;

private:
    Audio::stdSignal m_inputSignal;
    Audio::stdSignal m_transformedSignal;
    Audio::stdSignal m_outputSignal;

    QScopedPointer<Wavelet> m_wavelet;
};
