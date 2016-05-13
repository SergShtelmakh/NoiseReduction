#include "AudioSignal.h"
#include <QVector>

namespace {

Audio::SignalSource makeSignal(Aquila::WaveFile *wave)
{
    Audio::SignalSource resultSignal;
    for (auto sample : *wave) {
        resultSignal.push_back(sample);
    }
    return resultSignal;
}

}

AudioSignal::AudioSignal()
{
}

AudioSignal::AudioSignal(const QString &fileName)
{
    load(fileName);
}

AudioSignal::AudioSignal(const Audio::SignalSource &signalSource)
{
    m_signalSource = signalSource;
}

void AudioSignal::load(const QString &str)
{
    m_file.reset(new Aquila::WaveFile(QString(str).toStdString(), Aquila::StereoChannel::LEFT));

    m_signalSource = makeSignal(m_file.data());
}

void AudioSignal::save(const QString &str)
{
    auto frequency = m_file ? m_file->getSampleFrequency() : Audio::defaultSampleFrequency();

    Aquila::WaveFile::save(Aquila::SignalSource(m_signalSource.toStdVector(), frequency), QString(str).toStdString());
}

Audio::SignalSource AudioSignal::source() const
{
    return m_signalSource;
}

int AudioSignal::audioLength() const
{
    return m_file ? m_file->getAudioLength() : 0;
}

void AudioSignal::makeWhiteNoise()
{
    m_signalSource = Audio::makeWhiteNoise(m_signalSource, 10000);
}
