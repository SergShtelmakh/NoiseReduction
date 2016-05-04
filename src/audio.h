#pragma once

#include <QAudioEncoderSettings>
#include <QAudioFormat>

namespace Audio {

using SignalStd = std::vector<double>;
using SignalQt = QVector<qreal>;
using SignalsVectorStd = std::vector<SignalStd>;
using SignalsVectorQt = QVector<SignalQt>;

QAudioEncoderSettings encoderSettings();
QAudioFormat format();
SignalsVectorQt toQtVector(const SignalsVectorStd &vector);
double maxAmplitude(const SignalQt &signal);

}
