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
    static Audio::SignalSource makeSignal(Aquila::WaveFile *wave);

    AudioSignal();
    void load(const QString& str);
    void save(const QString& str);

    Audio::SignalSource source() const;
    void setSignalSource(const Audio::SignalSource& sign);

    int audioLength() const;

private:
    Audio::SignalSource m_signalSource;
    QScopedPointer<Aquila::WaveFile> m_file;
};
