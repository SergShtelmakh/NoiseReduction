#pragma once

#include <QWidget>

#include <src/audio/Audio.h>
#include <src/DenoisingManager.h>
#include <QScopedPointer>

class DecompositionItemWidget;

namespace Ui {
class DecompositionWidget;
}

class DecompositionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DecompositionWidget(bool noised = false,QWidget *parent = 0);
    ~DecompositionWidget();

private slots:
    void on_pbProcess_clicked();

private:
    void clearWidget();
    void processAudioSignal();
    QVector<double> thresholdsData() const;

    Ui::DecompositionWidget *ui;
    QVector<DecompositionItemWidget *> m_widgets;
    QScopedPointer<DenoisingManager> m_denoisingManager;
    int m_itemsCount = 0;
    QScopedPointer<AudioSignal> m_audioSignal;
};
