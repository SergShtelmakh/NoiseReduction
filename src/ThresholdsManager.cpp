#include "ThresholdsManager.h"
#include <QHash>
#include <QVector>

namespace {

using ThresholdType = ThresholdsManager::ThresholdType;
using ThresholdTypeHash = QHash<ThresholdType, QString>;

ThresholdTypeHash makeThresholdNameHash() {
    ThresholdTypeHash names;
    names[ThresholdType::Hard]  = "Hard";
    names[ThresholdType::Soft]  = "Soft";
    names[ThresholdType::Fuzzy] = "Fuzzy";
    return names;
}

static const auto thresholdsNameHash = makeThresholdNameHash();

QString toString(ThresholdType type) {
    return thresholdsNameHash.value(type, "");
}

}

QList<QString> ThresholdsManager::thresholdsNames()
{
    auto first = static_cast<int>(ThresholdType::First);
    auto last = static_cast<int>(ThresholdType::Last);
    QList<QString> names;
    for (auto i = first; i <= last; i++) {
        auto currentType = static_cast<ThresholdType>(i);
        names << toString(currentType);
    }
    return names;
}

ThresholdsManager::ThresholdType ThresholdsManager::fromString(const QString &str)
{
    return thresholdsNameHash.key(str, ThresholdType::Hard);
}

void ThresholdsManager::makeThreshold(const QVector<double> &thresholds)
{
    if (thresholds.size() != m_signalsVector.size()) {
        qDebug("Wrong thresholds vector size");
        return;
    }

    m_thresholdedSignalsVector.clear();
    for (int i = 0; i < thresholds.size(); ++i) {
        QVector<double> currentSignal;
        for (auto signalItem : m_signalsVector.at(i)) {
            switch (m_thresholdType) {
            case ThresholdType::Hard:
                currentSignal.append((qAbs(signalItem) > thresholds[i]) ? signalItem : 0);
                break;
            case ThresholdType::Soft:
                currentSignal.append(qMax(0.0, 1.0 - (thresholds[i]/ qAbs(signalItem + 0.0001))) * signalItem);
                break;
            default:
                break;
            }
        }
        m_thresholdedSignalsVector << currentSignal;
    }
}
