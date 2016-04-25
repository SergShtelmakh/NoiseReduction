#pragma once

#include <QString>
#include <QScopedPointer>

#include <libs/aquila/global.h>
#include <libs/aquila/source/WaveFile.h>

#include <src/Audio.h>

class AudioSignal
{

public:
    static Audio::stdSignal makeSignal(Aquila::WaveFile *wave);

    AudioSignal();
    void load(const QString& str);
    void save(const QString& str);

    Audio::stdSignal stdSignal() const;
    Audio::qtSignal qtSignal() const;
    void setSignal(const Audio::stdSignal& sign);

private:
    Audio::stdSignal m_signal;
    QScopedPointer<Aquila::WaveFile> m_file;

};
