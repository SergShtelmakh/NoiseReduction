#pragma once

#include "Wavelet.h"

class DiscretePeriodicWavelet : public Wavelet
{
public:
    DiscretePeriodicWavelet();
    ~DiscretePeriodicWavelet();

    WaveletTransformType type() override;

    void makeTransform(const Audio::SignalSource& signal) override;
    void makeThreshold(const QVector<double> &thresholds) override;
    void makeInverseTransform(const Audio::SignalSource& signal) override;

    QString resultText() override;
//    void denoising() override;
    Audio::SignalsSourceVector decomposition() override;

private:    
    std::vector<double> m_flag;
    std::vector<int> m_length;
};
