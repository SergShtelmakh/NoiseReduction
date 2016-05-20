#include "ThresholdsManager.h"

ThresholdsManager::ThresholdsManager()
{

}
//using ThresholdType = Wavelet::ThresholdType;
//using ThresholdTypeHash = QHash<ThresholdType, QString>;

//ThresholdTypeHash makeThresholdNameHash() {
//    ThresholdTypeHash names;
//    names[ThresholdType::Hard]  = "Hard";
//    names[ThresholdType::Soft]  = "Soft";
//    names[ThresholdType::Fuzzy] = "Fuzzy";
//    return names;
//}

//QList<QString> Wavelet::thresholdsNames()
//{
//    auto first = static_cast<int>(ThresholdType::First);
//    auto last = static_cast<int>(ThresholdType::Last);
//    QList<QString> names;
//    for (auto i = first; i <= last; i++) {
//        auto currentType = static_cast<ThresholdType>(i);
//        names << toString(currentType);
//    }
//    return names;
//}


//void DiscretePeriodicWavelet::makeThreshold(const QVector<double> &thresholds)
//{
//    if (m_transformedSignal.empty()) {
//        return;
//    }

//    m_thresholded.clear();

//    Q_ASSERT(thresholds.size() == static_cast<int>(m_length.size() - 1));

//    auto begin = 0;
//    auto end = 0;

//    for (size_t i = 0; i < m_length.size() - 1; ++i) {
//        auto currentSize = m_length[i];
//        end = begin + currentSize;
//        for (int j = begin; j < end; j++) {
//            double thresholded = 0.0;
//            switch (m_thresholdType) {
//            case ThresholdType::Hard:
//                thresholded = qAbs(m_transformedSignal[j]) > thresholds[i] ? m_transformedSignal[j] : 0;
//                break;
//            case ThresholdType::Soft:
//                thresholded = qMax(0.0, 1.0 - (thresholds[i]/ qAbs(m_transformedSignal[j] + 0.0001))) * m_transformedSignal[j];
//                break;
//            default:
//                break;
//            }


//            m_thresholded.push_back(thresholded);
//        }
//        begin = end;
//    }
//}
