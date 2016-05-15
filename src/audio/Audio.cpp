#include "Audio.h"
#include <QVector>
#include <QApplication>
#include <QTime>

namespace Audio {

int rand(int min, int max) {
    Q_ASSERT(max > min);
    return qrand() % ((max + 1) - min) + min;
}

QAudioEncoderSettings encoderSettings() {
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/pcm");
    audioSettings.setQuality(QMultimedia::HighQuality);
    return audioSettings;
}

QAudioFormat format() {
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    return format;
}

double maxAmplitude(const SignalSource &signal)
{
    double max = 0;
    for (auto val : signal) {
        max = qMax(max, qAbs(val));
    }

    return max;
}

SignalSource makeWhiteNoise(SignalSource &signal, double maxAmplitude)
{
    SignalSource result;
    for (auto sample : signal) {
        result << sample + rand(-maxAmplitude, maxAmplitude);
    }
    return result;
}

SignalSource makeThreshold(SignalSource &signal, double max)
{
    SignalSource result;
    for (auto i : signal) {
        auto sign = i > 0 ? 1 : -1;
        result << qMin(i, max) * sign;
    }
    return result;
}

double defaultSampleFrequency()
{
    return 48000;
}

QString generateAudioFileName()
{
    return qApp->applicationDirPath() + QString("/audio%1.wav").arg(QTime::currentTime().toString("hh_mm_ss_zzz"));
}

SignalSource makeSignalDifference(const SignalSource &first, const SignalSource &second)
{
    auto size = qMin(first.size(), second.size());
    SignalSource result;
    for (int i = 0 ; i < size; i++) {
        result.push_back(first[i] - second[i]);
    }
    return result;
}

}
