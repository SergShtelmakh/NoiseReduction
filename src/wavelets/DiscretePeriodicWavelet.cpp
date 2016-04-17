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
    m_inputSignal = signal;
    dwt(m_inputSignal, m_level, toStdString(m_waveletFunction), m_transformedSignal, m_flag, m_length);
}

void DiscretePeriodicWavelet::makeInverseTransform()
{
    idwt(m_transformedSignal, m_flag, toStdString(m_waveletFunction), m_resultSignal, m_length);
}

QString DiscretePeriodicWavelet::resultText()
{
    QString text = "Discrete periodic wavelet transform done\n\n";

    text += QString("Input signal size = %1\n").arg(m_inputSignal.size());
    text += QString("Transformed signal size = %1\n\n").arg(m_transformedSignal.size());

    for (size_t i = 0; i < m_flag.size(); i++) {
        text += QString("Flag[%1] = %2\n").arg(i).arg(m_flag[i]);
    }

    text += "\n";

    for (size_t i = 0; i < m_length.size(); i++) {
        text += QString("Length[%1] = %2\n").arg(i).arg(m_length[i]);
    }

    return text;
}

void DiscretePeriodicWavelet::denoising()
{
    auto currentSize = 0;
    auto begin = 0;
    auto end = 0;
    for (int i = 0; i < m_length.size() - 1; ++i) {
        currentSize = m_length[i];
        end = begin + currentSize;
        for (int j = begin; j < end; j++) {
            m_transformedSignal[j] = denoise(m_transformedSignal[j], i);
        }
        begin = end + 1;
    }
}

double DiscretePeriodicWavelet::denoise(double a, double lvl)
{
    return lvl < 3 ? a : 0;
}
