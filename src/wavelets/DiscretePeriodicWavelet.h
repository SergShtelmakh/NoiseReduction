#pragma once

#include "Wavelet.h"

class DiscretePeriodicWavelet : public Wavelet
{
public:
    DiscretePeriodicWavelet();
    ~DiscretePeriodicWavelet();

    WaveletTransformType type() override;
    void makeTransform(const Audio::SignalStd& signal) override;
    void makeInverseTransform(const Audio::SignalStd& signal) override;
    QString resultText() override;
    void denoising() override;
    Audio::SignalsVectorStd decomposition() override;

private:
    double denoise(double a, double lvl);

    std::vector<double> m_flag;
    std::vector<int> m_length;
};
