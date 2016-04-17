#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "PlotManager.h"
#include "AudioRecordWidget.h"

#include "Wavelet.h"

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

private:
    void log(const QString& str);

    Ui::MainWindow *ui;
    QScopedPointer<PlotManager> m_plotManager;
    QScopedPointer<AudioRecordWidget> m_recordWidget;
    QScopedPointer<Wavelet> m_wavelet;
};

#endif // MAINWINDOW_H
