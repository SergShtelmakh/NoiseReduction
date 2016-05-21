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
    static ThresholdType fromString(const QString& str);

    ThresholdsManager(){}

    ThresholdType thresholdType() const { return m_thresholdType; }
    void setThresholdType(const ThresholdType &thresholdType) { m_thresholdType = thresholdType; }

    Audio::SignalsSourceVector signalsVector() const { return m_signalsVector; }
    void setSignalsVector(const Audio::SignalsSourceVector &signalsVector) { m_signalsVector = signalsVector; }

    void makeThreshold(const QVector<double> &thresholds);

    Audio::SignalsSourceVector thresholdedSignalsVector() const { return m_thresholdedSignalsVector; }

private:
    ThresholdType m_thresholdType;

    Audio::SignalsSourceVector m_signalsVector;
    Audio::SignalsSourceVector m_thresholdedSignalsVector;

};
