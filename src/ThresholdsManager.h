#pragma once

#include <QString>

#include <src/audio/Audio.h>

class ThresholdsManager
{
public:
    enum ThresholdType {
        Hard  = 0,
        Soft  = 1,
        Fuzzy = 2,

        First = ThresholdType::Hard,
        Last  = ThresholdType::Fuzzy
    };

    static QList<QString> thresholdsNames();

    ThresholdsManager(){}

    ThresholdType thresholdType() const;
    void setThresholdType(const ThresholdType &thresholdType);

    void makeThreshold(const QVector<double> &thresholds);

    Audio::SignalsSourceVector signalsVector() const;
    void setSignalsVector(const Audio::SignalsSourceVector &signalsVector);

    Audio::SignalsSourceVector thresholdedSignalsVector() const;

private:
    ThresholdType m_thresholdType;

    Audio::SignalsSourceVector m_signalsVector;
    Audio::SignalsSourceVector m_thresholdedSignalsVector;

};
