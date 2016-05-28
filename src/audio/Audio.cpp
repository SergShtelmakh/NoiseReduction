#include "Audio.h"
#include <QVector>
#include <QApplication>
#include <QTime>
#include <QDebug>
#include <math.h>

namespace Audio {

int rand(int min, int max) {
    Q_ASSERT(max > min);
    return qrand() % ((max + 1) - min) + min;
}

double d_f(double x_1, double x_2, double x_3, double h) {
    return (x_3 - x_1)/(2*h);
}

double d2_f(double x_1, double x_2, double x_3, double h) {
    return (x_3 - 2*x_2 + x_1) / (h*h);
}

double gauss(double x) {
    const auto e = 2.718281;
    const auto a = 1.0;
    const auto b = 0.0;
    const auto c = 20.0;
    auto result = a*pow(e,-((x-b)*(x-b)/(2*c*c)));
    qDebug() << "Gauss func x =" << x << " resutl " << result;
    return result;
}

double inv_func(double x) {
    const double a = 100.0;
    return a/(a + x);
}

quint64 findDerivative(const Audio::SignalSource &source, double der, int h) {
    //qDebug() << "first " << source.at(0);
    for (int i = h; i < source.size() - h; i += h) {
        auto d = d_f(source[i - h], source[i], source[i + h], h);
        auto d2 = d2_f(source[i - h], source[i], source[i + h], h);
        //qDebug() << "i = " << i << " d = " << d << "source[0]/source[i]  " << source[0]/source[i];
        if ( der < d && source[0]/source[i] > 1.5 ) {
            return i;
        }
    }
    qDebug() << "Cannot find derivative";
    return -1;
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
    const int64_t maxRand = 1000;
    return rand(0, maxRand) < probability * maxRand;
}

SignalSource makeWhiteNoise(SignalSource &signal, double maxAmplitude, double probability)
{
    SignalSource result;
    for (auto sample : signal) {
        if (genenerateRandomEvent(probability)) {
            sample = sample + rand(-maxAmplitude, maxAmplitude);
        }
        result << sample;
    }
    return result;
}

SignalSource makeSignalDifference(const SignalSource &first, const SignalSource &second)
{
    auto size = qMin(first.size(), second.size());
    SignalSource result;
    for (int64_t i = 0 ; i < size; i++) {
        result.push_back(first[i] - second[i]);
    }
    return result;
}

QString generateAudioFileName(const QString &str)
{
    return QString("%1audio%2.wav").arg(str).arg(QTime::currentTime().toString("hh_mm_ss_zzz"));
}

SignalSource makeSignalDensity(const SignalSource &signal, bool positivePart, double max)
{
    double size_d = 0.0;
    for (auto val : signal) {
        size_d = positivePart ? qMax(size_d, val) : qMin(size_d, val);
    }

    auto size = static_cast<int64_t>(positivePart ? size_d : -size_d);
    size = qMin(size, static_cast<int64_t>(max));
    QVector<double> result(size);
    for (auto val : signal) {
        int64_t val_t = trunc(val);
        int64_t index = -1;
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

    int64_t sum = 0;
    if (positivePart) {
        for (int64_t i = result.size() - 1; i >= 0; --i) {
            sum += result[i];
            result[i] = sum;
        }
    } else {
        for (int64_t i = 0; i < result.size(); ++i) {
            sum += result[i];
            result[i] = sum;
        }
    }

    return result;
}

SignalSource makeAmplitudeFrequency(const SignalSource &signal, bool positivePart, int64_t step)
{
    double size_d = 0.0;
    for (auto val : signal) {
        size_d = positivePart ? qMax(size_d, val) : qMin(size_d, val);
    }

    auto size = static_cast<int64_t>(positivePart ? size_d : -size_d);
    QVector<double> result(size);
    for (auto val : signal) {
        int64_t val_t = trunc(val);
        int64_t index = -1;
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

    int64_t stepCounter = 0;
    int64_t sum = 0;
    QVector<double> compressedResult;
    for (int64_t i = 0; i < result.size(); i++) {
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

int64_t overThresholdsAmlitudeCount(const SignalSource &signal, double amplitude, int64_t maxCount) {
    int64_t count = 0;
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

double averageSignalDifference(const SignalSource &first, const SignalSource &second)
{
    auto size = qMin(first.size(), second.size());
    double sum;
    for (int i = 0 ; i < size; i++) {
        sum += qAbs(first[i] - second[i]);
    }
    return sum / size;
}

QVector<double> overThresholdsAmplitudeSum(const SignalSource &signal, double threshold, int neighbourhood)
{
    QVector<double> result;
    for (auto i = 0; i < signal.size(); i++) {
        double sum = 0.0;
        for (auto j = i - neighbourhood; j < i + neighbourhood; j++) {
            if (j >= 0 && j < signal.size() && qAbs(signal[j]) > threshold) {
                sum += qAbs(signal[j]) - threshold;
            }
        }
        result << sum;
    }
    return result;
}

double MSE(const SignalSource &signal1, const SignalSource &signal2)
{
    long double sum = 0;
    auto size = qMin(signal1.size(), signal2.size());
    for (auto i = 0; i < size; i++) {
        qDebug() << QString("%1 %2").arg(signal1[i]).arg(signal2[i]);
        auto s1 = gauss(signal1[i]);
        auto s2 = gauss(signal2[i]);
        auto g = (s1-s2)*(s1-s2);
        qDebug() << g;
        sum += g;
    }

    return sum / size;
}

QVector<double> averagePowers(const SignalSource &signal, int step, bool cleaning)
{
    int curItem = 0;
    QVector<double> result;
    long double sum = 0.0;
    for (double i : signal) {
        sum += qAbs(i);
        curItem++;
        if (curItem == step) {
            curItem = 0;
            auto val = sum/static_cast<double>(step);
//            qDebug() << "val " << static_cast<double>(val) << "   cleaning " << cleaning;
            val = ( val < 100 && cleaning) ? 0 : val;
//            qDebug() << "val " << static_cast<double>(val);
            result.append(val);
            sum = 0;
        }
    }
    return result;
}

double intervalMSE(const SignalSource &signal1, const SignalSource &signal2, int step)
{
    qDebug() << "create interval MSE";
    auto aver1 = averagePowers(signal1, step, false);
    auto aver2 = averagePowers(signal2, step, false);
    return MSE(aver1, aver2);
}


}
