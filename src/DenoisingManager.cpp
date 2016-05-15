#include "DenoisingManager.h"

DenoisingManager::DenoisingManager()
    : m_wavelet(Wavelet::create(Wavelet::WaveletTransformType::DiscretePeriodic1D))
{
}

void DenoisingManager::setSignal(const AudioSignal &signal)
{
    m_inputSignal = signal.source();
}

void DenoisingManager::makeThreshold(const QVector<double> &thresholds)
{
    m_wavelet->makeThreshold(thresholds);
}

void DenoisingManager::setWaveletName(const QString &waveletName)
{
    m_wavelet->setWaveletFunction(waveletName);
}

void DenoisingManager::setLevel(int level)
{
    m_wavelet->setLevel(level);
}

void DenoisingManager::setThresholdType(const QString &thresholdType)
{
    m_wavelet->setThresholdType(thresholdType);
}

Audio::SignalSource DenoisingManager::inputSignal() const
{
    return m_inputSignal;
}

Audio::SignalSource DenoisingManager::transformedSignal() const
{
    return m_transformedSignal;
}

Audio::SignalsSourceVector DenoisingManager::transformedDecomposition() const
{
    return m_wavelet->decomposition();
}

Audio::SignalSource DenoisingManager::outputSignal() const
{
    return m_outputSignal;
}

Audio::SignalSource DenoisingManager::thresholdedSignal() const
{
    return m_wavelet->thresholded();
}

void DenoisingManager::makeTransform()
{
    Q_ASSERT(m_wavelet);

    m_transformedSignal.clear();

    if (m_inputSignal.empty()) {
        return;
    }

    m_wavelet->makeTransform(m_inputSignal);
    m_transformedSignal = m_wavelet->transformedSignal();
}

void DenoisingManager::makeInverseTransform()
{
    Q_ASSERT(m_wavelet);

    m_outputSignal.clear();

    if (m_transformedSignal.empty()) {
        return;
    }

    m_wavelet->makeInverseTransform(m_transformedSignal);
    m_outputSignal = m_wavelet->resultSignal();
}
