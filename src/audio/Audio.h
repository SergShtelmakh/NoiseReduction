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
SignalSource makeWhiteNoise(SignalSource &signal, double maxAmplitude, double probability = 1);
SignalSource makeSignalDifference(const SignalSource &first, const SignalSource &second);
SignalSource makeSignalDensity(const SignalSource &signal, bool positivePart);
SignalSource makeAmplitudeFrequency(const SignalSource &signal, bool positivePart, int64_t step = 1);
int64_t overThresholdsAmlitudeCount(const SignalSource &signal, double amplitude, int64_t maxCount = -1);

QString generateAudioFileName(const QString &str = "");
}
