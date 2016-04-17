#ifndef WAVELET_H
#define WAVELET_H

#include <QList>
#include <QString>

class Wavelet
{
public:

    enum class WaveletType : int {
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

        First = WaveletType::Haar,
        Last  = WaveletType::Symmlet10
    };

    enum class WaveletTransformType {
        DiscretePeriodic1D  = 0,
        DiscreteSymmetric1D = 1,
        Stationary1D        = 2,

        First = WaveletTransformType::DiscretePeriodic1D,
        Last  = WaveletTransformType::Stationary1D
    };

    Wavelet();

public:
    void setWaveletType(WaveletType type);
    void setTransformType(WaveletTransformType type);
    void setLevel(int level);

    void makeTransform(std::vector<double> signal);
    void makeInverseTransform();

    static QList<QString> makeWaveletNames();
    static QList<QString> makeTransformNames();
    QString resultText();

    std::vector<double> input() const;
    void setInput(const std::vector<double> &input);

    std::vector<double> transform() const;

    std::vector<double> result() const;
    void setResult(const std::vector<double> &result);

private:
    WaveletType m_waveletType;
    WaveletTransformType m_transformType;
    int m_level;

    std::vector<double> m_input;
    std::vector<double> m_transform;
    std::vector<double> m_result;
    std::vector<double> m_flag;
    std::vector<int> m_length;
};

#endif // WAVELET_H
