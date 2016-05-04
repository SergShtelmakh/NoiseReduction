#ifndef DECOMPOSITIONITEM_H
#define DECOMPOSITIONITEM_H

#include <QWidget>

#include <src/Audio.h>

namespace Ui {
class DecompositionItem;
}

class DecompositionItem : public QWidget
{
    Q_OBJECT

public:
    explicit DecompositionItem(QWidget *parent = 0);
    ~DecompositionItem();

    Audio::SignalQt signal() const;
    void setSignal(const Audio::SignalQt &signal);

    double maxThreshold() const;
    void setMaxThreshold(double maxThreshold);

    double threshold() const;
    void setThreshold(double threshold);

signals:
    void signalChanged(const Audio::SignalQt &signal);
    void maxThresholdChanged(double maxThreshold);
    void thresholdChanged(double threshold);

public slots:

private:
    void updatePlotData();
    void replotSignal();
    void replotThreshold();

    Audio::SignalQt m_signal;
    double m_maxThreshold;
    double m_threshold;

    QVector<double> m_x;

    Ui::DecompositionItem *ui;
};

#endif // DECOMPOSITIONITEM_H
