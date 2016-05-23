#pragma once

#include <QMainWindow>

#include <src/audio/Audio.h>
#include <ThresholdTestWidget.h>
class AudioSignal;
class AudioRecordWidget;
class DenoisingWidget;
class AnalyzerWidget;

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionRecorder_triggered();

    void on_pbShowSourceSignal_clicked();
    void on_pbMakeWhiteNoise_clicked();
    void on_pbManualDenoising_clicked();
    void on_pbAutomaticDenoising_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_pbRevert_clicked();

    void on_pbThresholdsTest_clicked();

    void on_pushButton_clicked();

private:
    void updatePlot();

    Ui::MainWindow *ui;
    QScopedPointer<AudioSignal> m_sourceSignal;
    QScopedPointer<AudioSignal> m_processedSignal;
    QScopedPointer<AudioRecordWidget> m_recordWidget;
    QScopedPointer<DenoisingWidget> m_denoisingWidget;
    QScopedPointer<AnalyzerWidget> m_analyzerWidget;
    QScopedPointer<ThresholdTestWidget> m_thresholsdTestWidget;
};
