#include "DenoisingManager.h"

#include <QDebug>

#include <QTime>

DenoisingManager::DenoisingManager()
    : m_wavelet(Wavelet::create(Wavelet::WaveletTransformType::DiscretePeriodic1D))
{
}

void DenoisingManager::setSignal(const AudioSignal &signal)
{
    m_inputSignal = signal.source();
}

void DenoisingManager::denoise()
{
    makeTransform();

    // TODO thresholding

    makeInverseTransform();
}

void DenoisingManager::makeManualDenoise()
{
    makeTransform();

    m_widget.reset(new DecompositionWidget());
    m_widget->setDecomposition(m_wavelet->decomposition());
    m_widget->show();
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
