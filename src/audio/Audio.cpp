#include "Audio.h"
#include <QVector>
#include <QApplication>
#include <QTime>
#include <QDebug>

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

double defaultSampleFrequency()
{
    return 48000.0;
}

double maxAmplitude(const SignalSource &signal)
{
    double max = 0.0;
    for (auto val : signal) {
        max = qMax(max, qAbs(val));
    }

    return max;
}

bool genenerateRandomEvent(double probability)
{
    const int maxRand = 1000;
    return rand(0, maxRand) < probability * maxRand;
}

SignalSource makeWhiteNoise(SignalSource &signal, double maxAmplitude, double probability)
{
    SignalSource result;
    for (auto sample : signal) {
        if (genenerateRandomEvent(probability)) {
            sample += rand(-maxAmplitude, maxAmplitude);
        }
        result << sample;
    }
    return result;
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

QString generateAudioFileName()
{
    return qApp->applicationDirPath() + QString("/audio%1.wav").arg(QTime::currentTime().toString("hh_mm_ss_zzz"));
}

SignalSource makeSignalDensity(const SignalSource &signal, bool positivePart)
{
    double size_d = 0.0;
    for (auto val : signal) {
        size_d = positivePart ? qMax(size_d, val) : qMin(size_d, val);
    }

    auto size = static_cast<int>(positivePart ? size_d : -size_d);
    QVector<double> result(size);
    for (auto val : signal) {
        int val_t = trunc(val);
        int index = -1;
        if (val_t > 0 && positivePart) {
            index = val_t - 1;
        } else if (val_t < 0 && !positivePart) {
            index = size + val_t - 1;
        }

        if (index >= 0 && index < result.size()) {
            result[index] = result[index] + 1;
        } else {
//            qDebug() << "Wrong index " << index;
        }
    }

    int sum = 0;
    if (positivePart) {
        for (int i = result.size() - 1; i >= 0; --i) {
            sum += result[i];
            result[i] = sum;
        }
    } else {
        for (int i = 0; i < result.size(); ++i) {
            sum += result[i];
            result[i] = sum;
        }
    }

    return result;
}

SignalSource makeAmplitudeFrequency(const SignalSource &signal, bool positivePart, int step)
{
    double size_d = 0.0;
    for (auto val : signal) {
        size_d = positivePart ? qMax(size_d, val) : qMin(size_d, val);
    }

    auto size = static_cast<int>(positivePart ? size_d : -size_d);
    QVector<double> result(size);
    for (auto val : signal) {
        int val_t = trunc(val);
        int index = -1;
        if (val_t > 0 && positivePart) {
            index = val_t - 1;
        } else if (val_t < 0 && !positivePart) {
            index = size + val_t - 1;
        }

        if (index >= 0 && index < result.size()) {
            result[index] = result[index] + 1;
        } else {
//            qDebug() << "Wrong index " << index;
        }
    }

    int stepCounter = 0;
    int sum = 0;
    QVector<double> compressedResult;
    for (int i = 0; i < result.size(); i++) {
        stepCounter++;
        sum += result[i];
        if (stepCounter == step) {
            QVector<double> sumVector(step, sum);
            compressedResult.append(sumVector);
            stepCounter = 0;
            sum = 0;
        }
    }

    return compressedResult;
}

int overThresholdsAmlitudeCount(const SignalSource &signal, double amplitude, int maxCount) {
    int count = 0;
    for (auto signalItem : signal) {
        if (qAbs(signalItem) > amplitude ) {
            count++;
        }
        if (maxCount == count) {
            return count;
        }
    }

    return count;
}

}
