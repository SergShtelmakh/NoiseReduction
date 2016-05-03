#include "DenoisingManager.h"

#include <QTime>

namespace {
    int rand(int min, int max) {
        Q_ASSERT(max > min);
        return qrand() % ((max + 1) - min) + min;
    }
}

DenoisingManager::DenoisingManager()
    : m_wavelet(Wavelet::create(Wavelet::WaveletTransformType::DiscretePeriodic1D))
{
    qsrand(QTime::currentTime().msec());
}

void DenoisingManager::setSignal(const AudioSignal &signal)
{
    m_inputSignal = signal.stdSignal();
}

void DenoisingManager::makeTestNoise()
{
    for (auto sample : m_inputSignal) {
        sample = sample + rand(-100, 100);
    }
}

void DenoisingManager::denoise()
{
    m_wavelet->makeTransform(m_inputSignal);

    m_transformedSignal = m_wavelet->transformedSignal();

    // TODO thresholding

    m_wavelet->makeInverseTransform(m_transformedSignal);

    m_outputSignal = m_wavelet->resultSignal();

}

Audio::stdSignal DenoisingManager::inputSignal() const
{
    return m_inputSignal;
}

Audio::stdSignal DenoisingManager::transformedSignal() const
{
    return m_transformedSignal;
}

Audio::stdSignalsVector DenoisingManager::transformedDecomposition() const
{
    return m_wavelet->decomposition();
}

Audio::stdSignal DenoisingManager::outputSignal() const
{
    return m_outputSignal;
}
