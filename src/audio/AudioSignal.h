#pragma once

#include <QString>

#include <libs/aquila/global.h>
#include <libs/aquila/source/WaveFile.h>

#include <src/audio/Audio.h>

class AudioSignal
{

public:
    AudioSignal(){}

    AudioSignal(const QString& fileName);
    AudioSignal(const Audio::SignalSource& signalSource, double sampleFrequency = Audio::defaultSampleFrequency());
    void operator=(const AudioSignal& other);

    void load(const QString& fileName);
    void save(const QString& fileName);

    Audio::SignalSource source() const;
    int audioLength() const;

    void makeWhiteNoise(double maxAmplitude = 10000, double density = 1.0);

private:
    Audio::SignalSource m_signalSource;
    Aquila::WaveFile *m_file = nullptr;
    double m_sampleFrequency = Audio::defaultSampleFrequency();
};
