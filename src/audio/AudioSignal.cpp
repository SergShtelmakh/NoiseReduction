#include "AudioSignal.h"
#include <QVector>

Audio::Signal AudioSignal::makeSignal(Aquila::WaveFile *wave)
{
    Audio::Signal resultSignal;
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

    m_signal = makeSignal(m_file.data());
}

void AudioSignal::save(const QString &str)
{
    Aquila::WaveFile::save(Aquila::SignalSource(m_signal.toStdVector(), m_file->getSampleFrequency()), QString(str).toStdString());
}

Audio::Signal AudioSignal::signal() const
{
    return m_signal;
}

void AudioSignal::setSignal(const Audio::Signal &signal)
{
    m_signal = signal;
}

int AudioSignal::audioLength() const
{
    return m_file ? m_file->getAudioLength() : 0;
}
