#include "DiscretePeriodicWavelet.h"

#include <wavelet2d/wavelet2d.h>

DiscretePeriodicWavelet::DiscretePeriodicWavelet()
    : Wavelet()
{
}

DiscretePeriodicWavelet::~DiscretePeriodicWavelet()
{
}

Wavelet::WaveletTransformType DiscretePeriodicWavelet::type()
{
    return WaveletTransformType::DiscretePeriodic1D;
}

void DiscretePeriodicWavelet::makeTransform(const Signal &signal)
{
    m_flag.clear();
    m_length.clear();
    m_transformedSignal.clear();
    m_resultSignal.clear();
    auto input = signal;
    dwt(input, m_level, toStdString(m_waveletFunction), m_transformedSignal, m_flag, m_length);
}

void DiscretePeriodicWavelet::makeInverseTransform()
{
    idwt(m_transformedSignal, m_flag, toStdString(m_waveletFunction), m_resultSignal, m_length);
}

QString DiscretePeriodicWavelet::resultText()
{
    QString text = "Discrete periodic wavelet transform done\n\n";

    for (size_t i = 0; i < m_flag.size(); i++) {
        text += QString("Flag[%1] = %2\n").arg(i).arg(m_flag[i]);
    }

    text += "\n\n";

    for (size_t i = 0; i < m_length.size(); i++) {
        text += QString("Length[%1] = %2\n").arg(i).arg(m_length[i]);
    }

    return text;
}
