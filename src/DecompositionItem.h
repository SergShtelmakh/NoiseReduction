#ifndef DECOMPOSITIONITEM_H
#define DECOMPOSITIONITEM_H

#include <QWidget>

#include <src/audio/Audio.h>

namespace Ui {
class DecompositionItem;
}

class DecompositionItem : public QWidget
{
    Q_OBJECT

public:
    explicit DecompositionItem(QWidget *parent = 0);
    ~DecompositionItem();

    Audio::Signal signal() const;
    void setSignal(const Audio::Signal &signal);

    double maxThreshold() const;
    void setMaxThreshold(double maxThreshold);

    double threshold() const;
    void setThreshold(double threshold);

    Audio::Signal thresholded() const;

signals:
    void signalChanged(const Audio::Signal &signal);
    void maxThresholdChanged(double maxThreshold);
    void thresholdChanged(double threshold);

public slots:

private:
    void updatePlotData();
    void replotSignal();
    void replotThreshold();

    Audio::Signal m_signal;
    double m_maxThreshold;
    double m_threshold;

    QVector<double> m_x;

    Ui::DecompositionItem *ui;
};

#endif // DECOMPOSITIONITEM_H
