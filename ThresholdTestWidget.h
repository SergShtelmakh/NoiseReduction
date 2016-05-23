#ifndef THRESHOLDTESTWIDGET_H
#define THRESHOLDTESTWIDGET_H

#include <QWidget>
#include <src/audio/AudioSignal.h>
#include <src/PlotManager.h>

namespace Ui {
class ThresholdTestWidget;
}

class ThresholdTestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ThresholdTestWidget(QWidget *parent = 0);
    ~ThresholdTestWidget();

    void setSignal(const AudioSignal& signal);

private slots:
    void on_pbProcess_clicked();

    void on_sbLevel_valueChanged(int arg1);

private:
    Ui::ThresholdTestWidget *ui;

    AudioSignal m_inputSignal;
    PlotManager::PlotData m_plotData;
};

#endif // THRESHOLDTESTWIDGET_H
