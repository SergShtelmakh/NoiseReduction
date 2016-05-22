#include "ThresholdsManager.h"
#include <QHash>
#include <QVector>

namespace {

using ThresholdType = ThresholdsManager::ThresholdType;
using ThresholdTypeHash = QHash<ThresholdType, QString>;

const int cFuzzyStep = 5;

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

int nonZeroNeighborCount(const QVector<double> &data, int index) {
    int count = 0;
    for (int i = index - cFuzzyStep; i < index + cFuzzyStep; i++) {
        if (i >= 0 && i < data.size() && qAbs(data[i]) > 0) {
            count++;
        }
    }
    return count;
}

bool nonZeroNeighborExist(const QVector<double> &data, int index) {
    for (int i = index - cFuzzyStep; i < index + cFuzzyStep; i++) {
        if (i >= 0 && i < data.size() && qAbs(data[i]) > 0) {
            return true;
        }
    }
    return false;
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

QVector<double> ThresholdsManager::threshodedSignal(ThresholdsManager::ThresholdType type, const QVector<double> &signal, double threshod)
{
    QVector<double> result;
    switch (type) {
    case ThresholdType::Hard: {
        for (auto signalItem : signal) {
            result.append((qAbs(signalItem) > threshod) ? signalItem : 0);
        }
        break;
    }
    case ThresholdType::Soft: {
        for (auto signalItem : signal) {
            result.append(qMax(0.0, 1.0 - (threshod/ qAbs(signalItem + 0.0001))) * signalItem);
        }
        break;
    }
    case ThresholdType::Fuzzy: {
        for (auto signalItem : signal) {
            result.append(qMax(0.0, 1.0 - (threshod/ qAbs(signalItem + 0.0001))) * signalItem);
        }
        for (int i = 0; i < result.size(); i++) {
            if (qAbs(result[i]) > 0 && nonZeroNeighborCount(result, i) < cFuzzyStep) {
                result[i] = 0;
            }
        }
        break;
    }
    default:
        break;
    }

    return result;
}

void ThresholdsManager::makeThreshold(const QVector<double> &thresholds)
{
    if (thresholds.size() != m_signalsVector.size()) {
        qDebug("Wrong thresholds vector size");
        return;
    }

    m_thresholdedSignalsVector.clear();
    for (int i = 0; i < thresholds.size(); ++i) {
        m_thresholdedSignalsVector << threshodedSignal(m_signalsVector.at(i), thresholds[i]);
    }
}

QVector<double> ThresholdsManager::threshodedSignal(const QVector<double> &signal, double threshod)
{
    return threshodedSignal(m_thresholdType, signal, threshod);
}
