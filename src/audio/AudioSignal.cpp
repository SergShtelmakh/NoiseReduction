#include "AudioSignal.h"
#include <QVector>

Audio::SignalSource AudioSignal::makeSignal(Aquila::WaveFile *wave)
{
    Audio::SignalSource resultSignal;
    for (auto sample : *wave) {
        resultSignal.push_back(sample);
    }
    return resultSignal;
}

AudioSignal::AudioSignal()
{
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

void AudioSignal::setSignalSource(const Audio::SignalSource &signal)
{
    m_signalSource = signal;
}

int AudioSignal::audioLength() const
{
    return m_file ? m_file->getAudioLength() : 0;
}
