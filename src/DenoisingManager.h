#pragma once

#include <QObject>

#include <src/wavelets/wavelet.h>
#include <src/DecompositionWidget.h>

#include <QScopedPointer>

class DenoisingManager : public QObject
{
public:
    DenoisingManager();

    void setSignal(const AudioSignal& signal);

    void makeTestNoise();
    void denoise();

    void makeManualDenoise();

    Audio::SignalSource inputSignal() const;
    Audio::SignalSource transformedSignal() const;
    Audio::SignalsSourceVector transformedDecomposition() const;
    Audio::SignalSource outputSignal() const;

private:
    void makeTransform();
    void makeInverseTransform();

    Audio::SignalSource m_inputSignal;
    Audio::SignalSource m_transformedSignal;
    Audio::SignalSource m_outputSignal;

    QScopedPointer<Wavelet> m_wavelet;
    QScopedPointer<DecompositionWidget> m_widget;
};
