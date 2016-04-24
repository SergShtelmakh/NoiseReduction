#pragma once

#include "Wavelet.h"

class DiscretePeriodicWavelet : public Wavelet
{
public:
    DiscretePeriodicWavelet();
    ~DiscretePeriodicWavelet();

    WaveletTransformType type() override;
    void makeTransform(const Signal& signal) override;
    void makeInverseTransform(const Signal& signal) override;
    QString resultText() override;
    void denoising() override;

private:
    double denoise(double a, double lvl);

    std::vector<double> m_flag;
    std::vector<int> m_length;
};
