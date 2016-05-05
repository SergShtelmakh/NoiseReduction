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

    Audio::SignalStd inputSignal() const;
    Audio::SignalStd transformedSignal() const;
    Audio::SignalsVectorStd transformedDecomposition() const;
    Audio::SignalStd outputSignal() const;

private:
    void makeTransform();
    void makeInverseTransform();

    Audio::SignalStd m_inputSignal;
    Audio::SignalStd m_transformedSignal;
    Audio::SignalStd m_outputSignal;

    QScopedPointer<Wavelet> m_wavelet;
    QScopedPointer<DecompositionWidget> m_widget;
};
