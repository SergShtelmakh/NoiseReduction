#include "SignalAnalyzer.h"
#include <src/ThresholdsManager.h>
#include <QMetaType>

namespace {
    const int cLevel = 5;
    const ThresholdsManager::ThresholdType  cThresholdsType = ThresholdsManager::Soft;
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
    emit log("Processing started:\n");
    m_wavelet.setSignal(m_noisedSignal.source());
    emit log("Signal size " + QString("%1").arg(m_noisedSignal.source().size()));
    m_wavelet.setLevel(cLevel);
    emit log("Levels " + QString("%1").arg(cLevel));
    for (int curWavelet = Wavelet::Daubechies3; curWavelet <= Wavelet::Daubechies3; curWavelet++) {
        auto wavelet = static_cast<Wavelet::WaveletFunction>(curWavelet);
        log("\nWavelet " + Wavelet::toString(wavelet) + ":");
        m_wavelet.setWaveletFunction(wavelet);
        m_wavelet.makeTransform();

        double t = 5000;
//        for (int i = 0; i < 3; i++) {
//            t += 5000;
//            log("T = " + QString("%1").arg(t));
            auto thresholds = findOptimalThresholds(m_wavelet.transformedSignalVector(), t);
            emit optimalThresholdsFounded(Wavelet::toString(wavelet), thresholds);
            auto averageDiff = Audio::averageSignalDifference(m_inputSignal.source(), m_wavelet.outputSignal());
            log("Average" + QString("%1").arg(averageDiff));
//        }
    }
    emit finished();
}

QVector<double> SignalAnalyzer::findOptimalThresholds(const Audio::SignalsSourceVector &signalVector, double posibleOverthresholdsCount)
{
    auto thresholdedSignalVector = signalVector;
    QVector<double> result;
    for (int64_t i = 0 ; i < signalVector.size(); i++) {
        emit log("Level " + QString("%1").arg(i));
        auto min = 0;
        auto max = Audio::maxAmplitude(signalVector.at(i));
        auto val = min;
        while (qAbs(max - min) > 10) {
            val = (max + min) / 2;
            emit log(QString("Min %1, max %2, val = %3").arg(min).arg(max).arg(val));
            thresholdedSignalVector[i] = ThresholdsManager::threshodedSignal(cThresholdsType, signalVector.at(i), val);
            m_wavelet.setTransformedSignalVector(thresholdedSignalVector);
            m_wavelet.makeInverseTransform();
            auto diff = Audio::makeSignalDifference(m_noisedSignal.source(), m_wavelet.outputSignal());
            if (Audio::overThresholdsAmlitudeCount(diff, m_maxNoiseAmplitude, posibleOverthresholdsCount) < posibleOverthresholdsCount) {
                min = val;
            } else {
                max = val;
            }
        }
        result.prepend(val);
    }
    return result;
}
