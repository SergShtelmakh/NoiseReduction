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

void DiscretePeriodicWavelet::makeTransform(const Audio::SignalSource &signal)
{
    m_flag.clear();
    m_length.clear();
    m_transformedSignal.clear();
    m_resultSignal.clear();
    m_inputSignal = signal.toStdVector();
    dwt(m_inputSignal, m_level, toStdString(m_waveletFunction), m_transformedSignal, m_flag, m_length);
}

void DiscretePeriodicWavelet::makeThreshold(const QVector<double> &thresholds)
{
    if (m_transformedSignal.empty()) {
        return;
    }

    m_thresholded.clear();

    Q_ASSERT(thresholds.size() == static_cast<int>(m_length.size() - 1));

    auto begin = 0;
    auto end = 0;

    for (size_t i = 0; i < m_length.size() - 1; ++i) {
        auto currentSize = m_length[i];
        end = begin + currentSize;
        for (int j = begin; j < end; j++) {
            auto thresholded = qAbs(m_transformedSignal[j]) > thresholds[i] ? m_transformedSignal[j] : 0;
            m_thresholded.push_back(thresholded);
        }
        begin = end;
    }
}

void DiscretePeriodicWavelet::makeInverseTransform(const Audio::SignalSource &signal)
{
    if (m_transformedSignal.empty() && m_thresholded.empty())
        return;


    m_transformedSignal = signal.toStdVector();
    idwt(m_thresholded, m_flag, toStdString(m_waveletFunction), m_resultSignal, m_length);
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

Audio::SignalsSourceVector DiscretePeriodicWavelet::decomposition()
{
    if (m_transformedSignal.empty())
        return Audio::SignalsSourceVector();

    auto begin = 0;
    auto end = 0;
    Audio::SignalsSourceVector decomposition;
    for (size_t i = 0; i < m_length.size() - 1; ++i) {
        auto currentSize = m_length[i];
        Audio::SignalSource currentSignal;
        end = begin + currentSize;
        for (int j = begin; j < end; j++) {
            currentSignal.push_back(m_transformedSignal[j]);
        }
        decomposition.push_back(currentSignal);
        begin = end + 1;
    }

    return decomposition;
}

