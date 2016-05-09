#pragma once

#include <QObject>

#include <src/wavelets/wavelet.h>

#include <QScopedPointer>

class DenoisingManager : public QObject
{
public:
    DenoisingManager();

    void setSignal(const AudioSignal& signal);
    void makeThreshold(const QVector<double>& thr);

    void makeTestNoise();

    Audio::SignalSource inputSignal() const;
    Audio::SignalSource transformedSignal() const;
    Audio::SignalsSourceVector transformedDecomposition() const;
    Audio::SignalSource outputSignal() const;

    void makeTransform();
    void makeInverseTransform();

private:
    Audio::SignalSource m_inputSignal;
    Audio::SignalSource m_transformedSignal;
    Audio::SignalSource m_outputSignal;

    QScopedPointer<Wavelet> m_wavelet;
};
