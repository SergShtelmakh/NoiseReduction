#pragma once

#include <QAudioEncoderSettings>
#include <QAudioFormat>
#include <QVector>

namespace Audio {

using SignalSource = QVector<double>;
using SignalsSourceVector = QVector<SignalSource>;

// default values
QAudioEncoderSettings encoderSettings();
QAudioFormat format();
double defaultSampleFrequency();

// helpers
double maxAmplitude(const SignalSource &signal);
SignalSource makeWhiteNoise(SignalSource &signal, double maxAmplitude);
SignalSource makeSignalDifference(const SignalSource &first, const SignalSource &second);
SignalSource makeSignalDensity(const SignalSource &signal, bool positivePart);

QString generateAudioFileName();

}
