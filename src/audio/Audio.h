#pragma once

#include <QAudioEncoderSettings>
#include <QAudioFormat>

namespace Audio {

using SignalSource = QVector<qreal>;
using SignalsSourceVector = QVector<SignalSource>;

QAudioEncoderSettings encoderSettings();
QAudioFormat format();
double maxAmplitude(const SignalSource &signal);
SignalSource makeWhiteNoise(SignalSource &signal, double maxAmplitude);
SignalSource makeThreshold(SignalSource &signal, double max);
double defaultSampleFrequency();

}
