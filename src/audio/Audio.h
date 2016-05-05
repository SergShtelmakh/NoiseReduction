#pragma once

#include <QAudioEncoderSettings>
#include <QAudioFormat>

namespace Audio {

using SignalSource = QVector<qreal>;
using SignalsSourceVector = QVector<SignalSource>;

QAudioEncoderSettings encoderSettings();
QAudioFormat format();
double maxAmplitude(const SignalSource &signal);
void makeWhiteNoise(SignalSource &signal, double maxAmplitude);
void makeThreshold(SignalSource &signal, double max);

}
