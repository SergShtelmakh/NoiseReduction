#pragma once

#include <QString>
#include <QScopedPointer>

#include <libs/aquila/global.h>
#include <libs/aquila/source/WaveFile.h>

#include <src/Audio.h>

class AudioSignal
{

public:
    static Audio::SignalStd makeSignal(Aquila::WaveFile *wave);

    AudioSignal();
    void load(const QString& str);
    void save(const QString& str);

    Audio::SignalStd stdSignal() const;
    Audio::SignalQt qtSignal() const;
    void setSignal(const Audio::SignalStd& sign);

    int audioLength() const;

private:
    Audio::SignalStd m_signal;
    QScopedPointer<Aquila::WaveFile> m_file;

};
