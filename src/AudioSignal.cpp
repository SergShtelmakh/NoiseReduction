#include "AudioSignal.h"
#include <QVector>

AudioSignal::Signal AudioSignal::makeSignal(Aquila::WaveFile *wave)
{
    Signal result;
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

AudioSignal::Signal AudioSignal::signal_std() const
{
    return m_signal;
}

AudioSignal::QSignal AudioSignal::signal_qt() const
{
    return QVector<Aquila::SampleType>::fromStdVector(m_signal);
}
