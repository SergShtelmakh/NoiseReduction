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


quint64 findDerivative(const Audio::SignalSource &source, double der, int h);
// helpers
double maxAmplitude(const SignalSource &signal);
SignalSource makeWhiteNoise(SignalSource &signal, double maxAmplitude, double probability = 1);
SignalSource makeSignalDifference(const SignalSource &first, const SignalSource &second);
double averageSignalDifference(const SignalSource &first, const SignalSource &second);
SignalSource makeSignalDensity(const SignalSource &signal, bool positivePart, double max = 2000000);
SignalSource makeAmplitudeFrequency(const SignalSource &signal, bool positivePart, int64_t step = 1);
int64_t overThresholdsAmlitudeCount(const SignalSource &signal, double amplitude, int64_t maxCount = -1);
QVector<double> overThresholdsAmplitudeSum(const SignalSource &signal, double threshold, int neighbourhood);
int MSE(const SignalSource &signal1, const SignalSource &signal2);

QString generateAudioFileName(const QString &str = "");
}
