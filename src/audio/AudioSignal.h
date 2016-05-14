#pragma once

#include <QString>
#include <QScopedPointer>
#include <QVector>

#include <libs/aquila/global.h>
#include <libs/aquila/source/WaveFile.h>

#include <src/audio/Audio.h>

class AudioSignal
{

public:
    AudioSignal();
    AudioSignal(const QString& fileName);
    AudioSignal(const Audio::SignalSource& signalSource, double sampleFrequency = Audio::defaultSampleFrequency());

    void load(const QString& fileName);
    void save(const QString& fileName);

    Audio::SignalSource source() const;
    int audioLength() const;

    void makeWhiteNoise();

private:
    Audio::SignalSource m_signalSource;
    QScopedPointer<Aquila::WaveFile> m_file;
    double m_sampleFrequency = Audio::defaultSampleFrequency();
};
