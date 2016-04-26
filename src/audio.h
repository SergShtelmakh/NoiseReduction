#pragma once

#include <QAudioEncoderSettings>
#include <QAudioFormat>

namespace Audio {

using stdSignal = std::vector<double>;
using qtSignal = QVector<double>;
using stdSignalsVector = std::vector<stdSignal>;
using qtSignalsVector = QVector<qtSignal>;

QAudioEncoderSettings encoderSettings();
QAudioFormat format();

}
