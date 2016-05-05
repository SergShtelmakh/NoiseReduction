#include "Audio.h"
#include <QVector>

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

SignalsVectorQt toQtVector(const SignalsVectorStd &data)
{
    SignalsVectorQt vector;
    for (auto i : data) {
        vector << Signal::fromStdVector(i);
    }
    return vector;
}

double maxAmplitude(const Signal &signal)
{
    double max = 0;
    for (auto val : signal) {
        max = qMax(max, qAbs(val));
    }

    return max;
}

void makeWhiteNoise(Signal &signal, double maxAmplitude)
{
    for (auto sample : signal) {
        sample = sample + rand(-maxAmplitude, maxAmplitude);
    }
}

void makeThreshold(Signal &signal, double max) {
    for (auto i : signal) {
        auto sign = i > 0 ? 1 : -1;
        i = qMin(i, max) * sign;
    }
}

}
