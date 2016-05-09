#include "Wavelet.h"

#include <QHash>

#include "DiscretePeriodicWavelet.h"

using WaveletType = Wavelet::WaveletFunction;
using WaveletHash = QHash<WaveletType, std::string>;
using TransformType = Wavelet::WaveletTransformType;
using TransformTypeHash = QHash<TransformType, QString>;

uint qHash(WaveletType type, uint seed) {
    return qHash(static_cast<int>(type), seed);
}

uint qHash(TransformType type, uint seed) {
    return qHash(static_cast<int>(type), seed);
}

namespace {

WaveletHash makeWaveletNameHash() {
    WaveletHash names;
    names[WaveletType::Haar] = "haar";

    names[WaveletType::Daubechies1] = "db1";
    names[WaveletType::Daubechies2] = "db2";
    names[WaveletType::Daubechies3] = "db3";
    names[WaveletType::Daubechies4] = "db4";
    names[WaveletType::Daubechies5] = "db5";
    names[WaveletType::Daubechies6] = "db6";
    names[WaveletType::Daubechies7] = "db7";
    names[WaveletType::Daubechies8] = "db8";
    names[WaveletType::Daubechies9] = "db9";
    names[WaveletType::Daubechies10] = "db10";
    names[WaveletType::Daubechies11] = "db11";
    names[WaveletType::Daubechies12] = "db12";
    names[WaveletType::Daubechies13] = "db13";
    names[WaveletType::Daubechies14] = "db14";
    names[WaveletType::Daubechies15] = "db15";

    names[WaveletType::Biorthogonal1_1] = "bior1.1";
    names[WaveletType::Biorthogonal1_3] = "bior1.3";
    names[WaveletType::Biorthogonal1_5] = "bior1.5";
    names[WaveletType::Biorthogonal2_2] = "bior2.2";
    names[WaveletType::Biorthogonal2_4] = "bior2.4";
    names[WaveletType::Biorthogonal2_6] = "bior2.6";
    names[WaveletType::Biorthogonal2_8] = "bior2.8";
    names[WaveletType::Biorthogonal3_1] = "bior3.1";
    names[WaveletType::Biorthogonal3_3] = "bior3.3";
    names[WaveletType::Biorthogonal3_5] = "bior3.5";
    names[WaveletType::Biorthogonal3_7] = "bior3.7";
    names[WaveletType::Biorthogonal3_9] = "bior3.9";
    names[WaveletType::Biorthogonal4_4] = "bior4.4";
    names[WaveletType::Biorthogonal5_5] = "bior5.5";
    names[WaveletType::Biorthogonal6_8] = "bior6.8";

    names[WaveletType::Coiflet1] = "coif1";
    names[WaveletType::Coiflet2] = "coif2";
    names[WaveletType::Coiflet3] = "coif3";
    names[WaveletType::Coiflet4] = "coif4";
    names[WaveletType::Coiflet5] = "coif5";

    names[WaveletType::Symmlet1] = "sym1";
    names[WaveletType::Symmlet2] = "sym2";
    names[WaveletType::Symmlet3] = "sym3";
    names[WaveletType::Symmlet4] = "sym4";
    names[WaveletType::Symmlet5] = "sym5";
    names[WaveletType::Symmlet6] = "sym6";
    names[WaveletType::Symmlet7] = "sym7";
    names[WaveletType::Symmlet8] = "sym8";
    names[WaveletType::Symmlet9] = "sym9";
    names[WaveletType::Symmlet10] = "sym10";

    return names;
}

TransformTypeHash makeTransformNameHash() {
    TransformTypeHash names;
    names[TransformType::DiscretePeriodic1D] = "Discrete Periodic";
    return names;
}

}

Wavelet::Wavelet()
    : m_waveletFunction(WaveletFunction::Haar)
    , m_level(5)
{
}

Wavelet::~Wavelet()
{
}

void Wavelet::setWaveletFunction(Wavelet::WaveletFunction function)
{
    m_waveletFunction = function;
}


void Wavelet::setLevel(int level)
{
    m_level = level;
}

QList<QString> Wavelet::waveletFunctionsNames()
{
    auto first = static_cast<int>(WaveletFunction::First);
    auto last = static_cast<int>(WaveletFunction::Last);
    QList<QString> names;
    for (auto i = first; i <= last; i++) {
        auto currentType = static_cast<WaveletFunction>(i);
        names << toString(currentType);
    }
    return names;
}

QList<QString> Wavelet::makeTransformsNames()
{
    auto first = static_cast<int>(TransformType::First);
    auto last = static_cast<int>(TransformType::Last);
    QList<QString> names;
    for (auto i = first; i <= last; i++) {
        auto currentType = static_cast<TransformType>(i);
        names << toString(currentType);
    }
    return names;
}

Wavelet *Wavelet::create(Wavelet::WaveletTransformType type)
{
    switch (type) {
    case WaveletTransformType::DiscretePeriodic1D:
        return new DiscretePeriodicWavelet;
    default:
        Q_ASSERT(false);
    }

    return nullptr;
}

Audio::SignalSource Wavelet::transformedSignal() const
{
    return Audio::SignalSource::fromStdVector(m_transformedSignal);
}

Audio::SignalSource Wavelet::resultSignal() const
{
    return Audio::SignalSource::fromStdVector(m_resultSignal);
}

std::string Wavelet::toStdString(Wavelet::WaveletFunction function)
{
    static auto names = makeWaveletNameHash();
    return names.value(function, "");
}

QString Wavelet::toString(WaveletFunction function)
{
    return QString::fromStdString(toStdString(function));
}

QString Wavelet::toString(TransformType type) {
    static auto names = makeTransformNameHash();
    return names.value(type, "");
}
