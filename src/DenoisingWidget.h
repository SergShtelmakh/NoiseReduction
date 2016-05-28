#pragma once

#include <QWidget>

#include <src/audio/AudioSignal.h>
#include <QScopedPointer>

class DenoisingManager;
class ThresholdsWidget;

namespace Ui { class DenoisingWidget; }

class DenoisingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DenoisingWidget(QWidget *parent = 0);
    ~DenoisingWidget();

    void setSignal(const AudioSignal& signal);
    void setOriginalSignal(const AudioSignal& signal);

private slots:
    void on_pbPrepare_clicked();
    void on_pbProcess_clicked();

private:
    void clearWidget();
    QVector<double> thresholdsData() const;

    Ui::DenoisingWidget *ui;
    QVector<ThresholdsWidget *> m_widgets;
    QScopedPointer<DenoisingManager> m_denoisingManager;
    QScopedPointer<AudioSignal> m_inputAudioSignal;
    QScopedPointer<AudioSignal> m_outputAudioSignal;

    QScopedPointer<AudioSignal> m_originalAudioSignal;
    int m_itemsCount = 0;
};
