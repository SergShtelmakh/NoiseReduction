#include "DenoisingManager.h"

DenoisingManager::DenoisingManager()
    : m_wavelet(new DiscretePeriodicWavelet())
    , m_thresholdsManager(new ThresholdsManager())
{
}

void DenoisingManager::setSignal(const AudioSignal &signal)
{
    m_inputSignal = signal.source();
}

void DenoisingManager::prepareToDenoising(const QString &waveletName, int level)
{
    m_wavelet->setWaveletFunction(Wavelet::fromString(waveletName));
    m_wavelet->setLevel(level);
    m_wavelet->setSignal(m_inputSignal);

    m_wavelet->makeTransform();
    m_transformedSignal = m_wavelet->transformedSignal();
}

void DenoisingManager::denoising(const QString &thresholdType, const QVector<double> &thresholds)
{
    m_thresholdsManager->setSignalsVector(m_wavelet->transformedSignalVector());
    m_thresholdsManager->setThresholdType(ThresholdsManager::fromString(thresholdType));
    m_thresholdsManager->makeThreshold(thresholds);

    m_wavelet->setTransformedSignalVector(m_thresholdsManager->thresholdedSignalsVector());

    m_wavelet->makeInverseTransform();
    m_outputSignal = m_wavelet->outputSignal();
}

Audio::SignalsSourceVector DenoisingManager::transformedDecomposition() const
{
    return m_wavelet->transformedSignalVector();
}

Audio::SignalSource DenoisingManager::thresholdedSignal() const
{
    return m_wavelet->transformedSignal();
}

