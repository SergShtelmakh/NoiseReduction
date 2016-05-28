#include "SignalAnalyzer.h"
#include <src/ThresholdsManager.h>
#include <QMetaType>
#include <src/PlotManager.h>

namespace {
const int cLevel = 7;
const ThresholdsManager::ThresholdType  cThresholdsType = ThresholdsManager::Fuzzy;
const double fi = 1.618;
}

SignalAnalyzer::SignalAnalyzer(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<QVector<double>>();
}

void SignalAnalyzer::setData(const AudioSignal &inputSignal, const AudioSignal &noisedSignal, double maxNoiseAmplitude)
{
    m_inputSignal = inputSignal;
    m_noisedSignal = noisedSignal;
    m_maxNoiseAmplitude = maxNoiseAmplitude;
}

void SignalAnalyzer::start()
{
    emit started();
    m_stoped = false;
    emit log("Processing started:\n");
    m_wavelet.setSignal(m_noisedSignal.source());
    emit log("Signal size " + QString("%1").arg(m_noisedSignal.source().size()));
    m_wavelet.setLevel(cLevel);
    emit log("Levels " + QString("%1").arg(cLevel));
    for (int curWavelet = Wavelet::Daubechies1; curWavelet <= Wavelet::Last; curWavelet++) {
        auto wavelet = static_cast<Wavelet::WaveletFunction>(curWavelet);
        log("\nWavelet " + Wavelet::toString(wavelet) + ":");
        m_wavelet.setWaveletFunction(wavelet);
        m_wavelet.makeTransform();

        double t = 30;
//                auto thresholds = findOptimalThresholds(m_wavelet.transformedSignalVector(), t);
//                emit optimalThresholdsFounded(Wavelet::toString(wavelet), thresholds);
        testMSE(m_wavelet.transformedSignalVector(), t);
        if (m_stoped) {
            emit finished();
            return;
        }
        auto averageDiff = Audio::averageSignalDifference(m_inputSignal.source(), m_wavelet.outputSignal());
        log("Average" + QString("%1").arg(averageDiff));
        log("MSE " + QString("%1").arg(Audio::MSE(m_inputSignal.source(), m_wavelet.outputSignal())));
        //        }
    }
    emit finished();
}

void SignalAnalyzer::stop()
{
    m_stoped = true;
}

QVector<double> SignalAnalyzer::findOptimalThresholds(const Audio::SignalsSourceVector &signalVector, double count)
{
    QVector<double> result;
    auto min_mse = calculateMSE(signalVector,0,0);
    emit log("Start MSE " + QString("%1").arg(min_mse));
    for (int64_t i = 0 ; i < signalVector.size(); i++) {
        emit log("Level " + QString("%1").arg(i));
        auto min = 0;
        auto max = Audio::maxAmplitude(signalVector.at(i));
        auto max_mse = calculateMSE(signalVector, i, max);
        emit log("Max MSE " + QString("%1").arg(max_mse));
        auto x1 = max - (max - min)/fi;
        auto x2 = min + (max - min)/fi;
        while (qAbs(max - min) > 10 && !m_stoped) {
            emit log(QString("Min %1, max %2, val1 = %3, val2 = %4").arg(min).arg(max).arg(x1).arg(x2));

            auto new_mse1 = calculateMSE(signalVector, i, x1);
            auto new_mse2 = calculateMSE(signalVector, i, x2);
            emit log(QString("mse1 = %1  mse2 = %2\n").arg(new_mse1).arg(new_mse2));
            if (new_mse1 >= new_mse2) {
                min = x1;
                x1 = x2;
                x2 = min + (max - min)/fi;
            } else {
                max = x2;
                x2 = x1;
                x1 = max - (max - min)/fi;
            }
        }
        emit log(QString("\n\nresult = %1").arg((min + max)/2));
        result.prepend((min + max)/2);
    }
    return result;
}

double SignalAnalyzer::calculateMSE(const Audio::SignalsSourceVector &ssv, int level, double val)
{
    Audio::SignalsSourceVector s = ssv;
    s[level] = ThresholdsManager::threshodedSignal(cThresholdsType, ssv.at(level), val);
    m_wavelet.setTransformedSignalVector(s);
    m_wavelet.makeInverseTransform();
    return Audio::intervalMSE(m_inputSignal.source(), m_wavelet.outputSignal(), 1000);
}

void SignalAnalyzer::testMSE(const Audio::SignalsSourceVector &signalVector, double count) {
    for (int64_t i = 0 ; i < signalVector.size(); i++) {
        emit log("Level " + QString("%1").arg(i));
        auto min = 0;
        auto max = Audio::maxAmplitude(signalVector.at(i))/10;
        QVector<double> mseVector;
        for (int j = 0; j < count; j++) {
            auto val = (j + 1)*(max - min)/count;

            if (m_stoped) {
                return;
            }

            auto new_mse = calculateMSE(signalVector, i, val);
            mseVector.append(new_mse);
            log(QString("%1").arg(new_mse));
        }
        emit mseCalculated(mseVector);
    }
}
