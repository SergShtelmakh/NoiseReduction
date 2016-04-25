#pragma once

#include <QAudioEncoderSettings>
#include <QAudioFormat>

namespace Audio {

using stdSignal = std::vector<double>;
using qtSignal = QVector<double>;

QAudioEncoderSettings encoderSettings();
QAudioFormat format();

}
