#include "Wavelet.h"

#include <QHash>

#include <wavelet2d/wavelet2d.h>

using WaveletType = Wavelet::WaveletType;
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

std::string toStdString(WaveletType type) {
    static auto names = makeWaveletNameHash();
    return names.value(type, "");
}

QString toString(WaveletType type) {
    return QString::fromStdString(toStdString(type));
}

TransformTypeHash makeTransformNameHash() {
    TransformTypeHash names;
    names[TransformType::DiscretePeriodic1D] = "Discrete Periodic";
    names[TransformType::DiscreteSymmetric1D] = "Discrete Symmetric";
    names[TransformType::Stationary1D] = "Stationary";

    return names;
}

QString toString(TransformType type) {
    static auto names = makeTransformNameHash();
    return names.value(type, "");
}

}

Wavelet::Wavelet()
    : m_waveletType(WaveletType::Haar)
    , m_transformType(WaveletTransformType::DiscretePeriodic1D)
    , m_level(1)
{
}

void Wavelet::setWaveletType(Wavelet::WaveletType type)
{
    m_waveletType = type;
}

void Wavelet::setTransformType(Wavelet::WaveletTransformType type)
{
    m_transformType = type;
}

void Wavelet::setLevel(int level)
{
    m_level = level;
}

void Wavelet::makeTransform(std::vector<double> signal)
{
    m_input = signal;
    m_flag.clear();
    m_length.clear();
    m_transform.clear();
    m_result.clear();

    switch (m_transformType) {
    case WaveletTransformType::DiscretePeriodic1D:
        dwt(m_input, m_level, toStdString(m_waveletType), m_transform, m_flag, m_length);
        break;
    case WaveletTransformType::DiscreteSymmetric1D:
        dwt_sym(m_input, m_level, toStdString(m_waveletType), m_transform, m_flag, m_length);
        break;
    case WaveletTransformType::Stationary1D:
        swt(m_input, m_level, toStdString(m_waveletType), m_transform, m_length[0]);
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

void Wavelet::makeInverseTransform()
{
    switch (m_transformType) {
    case WaveletTransformType::DiscretePeriodic1D:
        idwt(m_transform, m_flag, toStdString(m_waveletType), m_result, m_length);
        break;
    case WaveletTransformType::DiscreteSymmetric1D:
        idwt_sym(m_transform, m_flag, toStdString(m_waveletType), m_result, m_length);
        break;
    case WaveletTransformType::Stationary1D:
        iswt(m_transform, m_level, toStdString(m_waveletType), m_result);
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

QList<QString> Wavelet::makeWaveletNames()
{
    auto first = static_cast<int>(WaveletType::First);
    auto last = static_cast<int>(WaveletType::Last);
    QList<QString> names;
    for (auto i = first; i <= last; i++) {
        auto currentType = static_cast<WaveletType>(i);
        names << toString(currentType);
    }
    return names;
}

QList<QString> Wavelet::makeTransformNames()
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

QString Wavelet::resultText()
{
    QString text;

    for (size_t i = 0; i < m_flag.size(); i++) {
        text.append(QString("Flag[%1] = %2\n").arg(i).arg(m_flag[i]));
    }

    text.append("\n\n");

    for (size_t i = 0; i < m_length.size(); i++) {
        text.append(QString("Length[%1] = %2\n").arg(i).arg(m_length[i]));
    }

    return text;

}

std::vector<double> Wavelet::input() const
{
    return m_input;
}

void Wavelet::setInput(const std::vector<double> &input)
{
    m_input = input;
}

std::vector<double> Wavelet::transform() const
{
    return m_transform;
}

std::vector<double> Wavelet::result() const
{
    return m_result;
}

void Wavelet::setResult(const std::vector<double> &result)
{
    m_result = result;
}
