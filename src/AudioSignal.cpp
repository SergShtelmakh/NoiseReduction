#include "AudioSignal.h"
#include <QVector>

Audio::SignalStd AudioSignal::makeSignal(Aquila::WaveFile *wave)
{
    Audio::SignalStd result;
    for (auto sample : *wave) {
        result.push_back(sample);
    }
    return result;
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
    Aquila::WaveFile::save(Aquila::SignalSource(m_signal, m_file->getSampleFrequency()), QString(str).toStdString());
}

Audio::SignalStd AudioSignal::stdSignal() const
{
    return m_signal;
}

Audio::SignalQt AudioSignal::qtSignal() const
{
    return QVector<Aquila::SampleType>::fromStdVector(m_signal);
}

void AudioSignal::setSignal(const Audio::SignalStd &sign)
{
    m_signal = sign;
}

int AudioSignal::audioLength() const
{
    return m_file ? m_file->getAudioLength() : 0;
}
