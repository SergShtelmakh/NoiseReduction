#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <PlotManager.h>

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

private:
    Ui::MainWindow *ui;
    QScopedPointer<PlotManager> m_plotManager;
};

#endif // MAINWINDOW_H
