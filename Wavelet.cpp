#include "Wavelet.h"

#include <QHash>

using WaveletType = Wavelet::WaveletType;
using WaveletHash = QHash<WaveletType, std::string>;

uint qHash(WaveletType type, uint seed) {
    return qHash(static_cast<int>(type), seed);
}

namespace {

WaveletHash makeWaveletNameHash() {
    WaveletHash names;
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

}

std::string toStdString(WaveletType type) {
    static auto names = makeWaveletNameHash();
    return names.value(type, "");
}


Wavelet::Wavelet()
{

}

void Wavelet::makeTransform()
{

}

void Wavelet::makeInverseTransform()
{

}
