#pragma once

#include <QMainWindow>

#include <src/audio/Audio.h>

class AudioSignal;
class AudioRecordWidget;
class DenoisingWidget;

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
    void on_actionRecorder_triggered();

    void on_pbShowSourceSignal_clicked();
    void on_pbMakeWhiteNoise_clicked();
    void on_pbManualDenoising_clicked();
    void on_pbAutomaticDenoising_clicked();

private:
    Ui::MainWindow *ui;
    QScopedPointer<AudioSignal> m_signal;
    QScopedPointer<AudioRecordWidget> m_recordWidget;
    QScopedPointer<DenoisingWidget> m_denoisingWidget;
};
