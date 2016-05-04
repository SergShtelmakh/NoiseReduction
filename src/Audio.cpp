#include "Audio.h"
#include <QVector>

namespace Audio {

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
        vector << SignalQt::fromStdVector(i);
    }
    return vector;
}

double maxAmplitude(const SignalQt &signal)
{
    double max = 0;
    for (auto val : signal) {
        max = qMax(max, qAbs(val));
    }

    return max;
}

}
