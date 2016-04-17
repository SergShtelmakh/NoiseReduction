#pragma once

#include <QList>
#include <QString>

using Signal = std::vector<double>;

class Wavelet
{
public:
    enum class WaveletFunction : int {
        Haar = 0,

        Daubechies1 = 1,
        Daubechies2 = 2,
        Daubechies3 = 3,
        Daubechies4 = 4,
        Daubechies5 = 5,
        Daubechies6 = 6,
        Daubechies7 = 7,
        Daubechies8 = 8,
        Daubechies9 = 9,
        Daubechies10 = 10,
        Daubechies11 = 11,
        Daubechies12 = 12,
        Daubechies13 = 13,
        Daubechies14 = 14,
        Daubechies15 = 15,

        Biorthogonal1_1 = 16,
        Biorthogonal1_3 = 17,
        Biorthogonal1_5 = 18,
        Biorthogonal2_2 = 19,
        Biorthogonal2_4 = 20,
        Biorthogonal2_6 = 21,
        Biorthogonal2_8 = 22,
        Biorthogonal3_1 = 23,
        Biorthogonal3_3 = 24,
        Biorthogonal3_5 = 25,
        Biorthogonal3_7 = 26,
        Biorthogonal3_9 = 27,
        Biorthogonal4_4 = 28,
        Biorthogonal5_5 = 29,
        Biorthogonal6_8 = 30,

        Coiflet1 = 31,
        Coiflet2 = 32,
        Coiflet3 = 33,
        Coiflet4 = 34,
        Coiflet5 = 35,

        Symmlet1 = 36,
        Symmlet2 = 37,
        Symmlet3 = 38,
        Symmlet4 = 39,
        Symmlet5 = 40,
        Symmlet6 = 41,
        Symmlet7 = 42,
        Symmlet8 = 43,
        Symmlet9 = 44,
        Symmlet10 = 45,

        First = WaveletFunction::Haar,
        Last  = WaveletFunction::Symmlet10
    };

    enum class WaveletTransformType {
        DiscretePeriodic1D  = 0,
        DiscreteSymmetric1D = 1,
        Stationary1D        = 2,

        First = WaveletTransformType::DiscretePeriodic1D,
        Last  = WaveletTransformType::Stationary1D
    };

    Wavelet();
    virtual ~Wavelet();

public:
    static QList<QString> waveletFunctionsNames();
    static QList<QString> makeTransformsNames();

    static Wavelet *create(WaveletTransformType type);

    virtual WaveletTransformType type() = 0;
    virtual void makeTransform(const Signal& signal) = 0;
    virtual void makeInverseTransform() = 0;
    virtual QString resultText() = 0;

    void setWaveletFunction(WaveletFunction function);
    void setLevel(int level);

    Signal transformedSignal() const;
    Signal resultSignal() const;

protected:
    static std::string toStdString(WaveletFunction function);
    static QString toString(WaveletFunction function);
    static QString toString(WaveletTransformType type);

    WaveletFunction m_waveletFunction;
    int m_level;
    Signal m_transformedSignal;
    Signal m_resultSignal;

private:
};