#pragma once

#include <QAudioEncoderSettings>
#include <QAudioFormat>

namespace Audio {

using SignalStd = std::vector<double>;
using Signal = QVector<qreal>;
using SignalsVectorStd = std::vector<SignalStd>;
using SignalsVectorQt = QVector<Signal>;

QAudioEncoderSettings encoderSettings();
QAudioFormat format();
SignalsVectorQt toQtVector(const SignalsVectorStd &vector);
double maxAmplitude(const Signal &signal);
void makeWhiteNoise(Signal &signal, double maxAmplitude);
void makeThreshold(Signal &signal, double max);

}
