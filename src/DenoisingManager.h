#pragma once

#include <QObject>

#include <src/AudioSignal.h>
#include <src/wavelets/wavelet.h>

#include <QScopedPointer>

class DenoisingManager : public QObject
{
public:
    DenoisingManager();

    void setSignal(const AudioSignal& signal);

    void makeTestNoise();
    void denoise();

    AudioSignal::Signal inputSignal() const;

    AudioSignal::Signal transformedSignal() const;

    AudioSignal::Signal outputSignal() const;

private:
    AudioSignal::Signal m_inputSignal;
    AudioSignal::Signal m_transformedSignal;
    AudioSignal::Signal m_outputSignal;

    QScopedPointer<Wavelet> m_wavelet;
};
