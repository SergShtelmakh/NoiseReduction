#pragma once

#include "Wavelet.h"

class DiscretePeriodicWavelet : public Wavelet
{
public:
    DiscretePeriodicWavelet();
    ~DiscretePeriodicWavelet();

    WaveletTransformType type() override;
    void makeTransform(const Signal& signal) override;
    void makeInverseTransform() override;
    QString resultText() override;

private:
    std::vector<double> m_flag;
    std::vector<int> m_length;
};
