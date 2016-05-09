#pragma once

#include <QMainWindow>

#include <src/audio/Audio.h>
#include <src/PlotManager.h>

class AudioSignal;
class AudioRecordWidget;
class PlotManager;
class DenoisingManager;
class DecompositionWidget;

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

    void makePlot(PlotType type, const AudioSignal& signal);
    QCustomPlot *getWidgetForPlot(PlotType type);

    Ui::MainWindow *ui;
    QScopedPointer<PlotManager> m_plotManager;
    QScopedPointer<AudioRecordWidget> m_recordWidget;
    QScopedPointer<AudioSignal> m_testSignal;
    QScopedPointer<AudioSignal> m_noisedSignal;
//    QScopedPointer<DenoisingManager> m_denoisingManager;
    QScopedPointer<DecompositionWidget> m_decompositionWidget;
};
