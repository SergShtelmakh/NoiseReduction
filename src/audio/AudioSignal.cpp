#include "AudioSignal.h"
#include <QVector>

namespace {

const int cMsInSec = 1000;

Audio::SignalSource makeSignal(Aquila::WaveFile *wave)
{
    Audio::SignalSource resultSignal;
    for (auto sample : *wave) {
        resultSignal.push_back(sample);
    }
    return resultSignal;
}

}

AudioSignal::AudioSignal(const QString &fileName)
{
    load(fileName);
}

AudioSignal::AudioSignal(const Audio::SignalSource &signalSource, double sampleFrequency)
{
    m_signalSource = signalSource;
    m_sampleFrequency = sampleFrequency;
}

void AudioSignal::operator=(const AudioSignal &other)
{
    m_file = &(*other.m_file);
    m_sampleFrequency = other.m_sampleFrequency;
    m_signalSource = other.m_signalSource;
}

void AudioSignal::load(const QString &str)
{
    if (m_file) {
        delete m_file;
    }
    m_file = new Aquila::WaveFile(QString(str).toStdString(), Aquila::StereoChannel::LEFT);

    m_signalSource = makeSignal(m_file);
}

void AudioSignal::save(const QString &str)
{
    auto frequency = m_file ? m_file->getSampleFrequency() : Audio::defaultSampleFrequency();
    for (auto i = 1; i < m_signalSource.size() - 1; i++) {
        if (m_signalSource[i] >= INT16_MAX) {
            m_signalSource[i] = m_signalSource[i - 1];
        }
        if (m_signalSource[i] <= INT16_MIN) {
           m_signalSource[i] = m_signalSource[i - 1];
        }
    }
    Aquila::WaveFile::save(Aquila::SignalSource(m_signalSource.toStdVector(), frequency), QString(str).toStdString());
}

Audio::SignalSource AudioSignal::source() const
{
    return m_signalSource;
}

int AudioSignal::audioLength() const
{
    return m_file ? m_file->getAudioLength() : m_signalSource.size() / (m_sampleFrequency / cMsInSec);
}

void AudioSignal::makeWhiteNoise(double maxAmplitude, double density)
{
    m_signalSource = Audio::makeWhiteNoise(m_signalSource, maxAmplitude, density);
}
