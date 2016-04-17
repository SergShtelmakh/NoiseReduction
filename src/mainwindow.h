#pragma once

#include <QMainWindow>

#include "PlotManager.h"
#include "AudioRecordWidget.h"

#include <aquila/source/WaveFile.h>

#include <src/wavelets/Wavelet.h>

using Signal = std::vector<Aquila::SampleType>;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbStart_clicked();

    void on_pbStop_clicked();

    void on_actionRecorder_triggered();

    void on_cbWaveletType_currentIndexChanged(int index);

    void on_leLevel_textChanged(const QString &arg1);

    void on_cbTransformType_currentIndexChanged(int index);

private:
    enum class PlotType {
        InputSignal,
        InputSignalTransformed,
        NoiseSignal,
        NoiseSignalTransformed,
        ResultSignal
    };

    enum class SignalForTransform {
        Input,
        Noise
    };

    void clearLog();
    void log(const QString& str);

    void makePlot(PlotType type,const Aquila::WaveFile& file);
    void makePlot(PlotType type,const Signal& signal);
    void makeTransform(SignalForTransform sigType, const Signal& signal);
    QCustomPlot *getWidgetForPlot(PlotType type);

    Ui::MainWindow *ui;
    QScopedPointer<PlotManager> m_plotManager;
    QScopedPointer<AudioRecordWidget> m_recordWidget;
    QScopedPointer<Wavelet> m_signalWavelet;
    QScopedPointer<Wavelet> m_noiseWavelet;
};
