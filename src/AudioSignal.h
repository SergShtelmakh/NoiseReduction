#pragma once

#include <QString>
#include <QScopedPointer>

#include <libs/aquila/global.h>
#include <libs/aquila/source/WaveFile.h>

class AudioSignal
{

public:
    using Signal  = std::vector<Aquila::SampleType>;
    using QSignal = QVector<Aquila::SampleType>;
    static Signal makeSignal(Aquila::WaveFile *wave);

    AudioSignal();
    void load(const QString& str);
    void save(const QString& str);

    Signal signal_std() const;
    QSignal signal_qt() const;
    void setSignal(const Signal& sign);

private:
    Signal m_signal;
    QScopedPointer<Aquila::WaveFile> m_file;

};
