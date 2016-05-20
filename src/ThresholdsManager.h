#pragma once

#include <QString>

class ThresholdsManager
{
public:
    enum class ThresholdType {
        Hard  = 0,
        Soft  = 1,
        Fuzzy = 2,

        First = ThresholdType::Hard,
        Last  = ThresholdType::Fuzzy
    };

    static QList<QString> thresholdsNames();

    ThresholdsManager();
};
