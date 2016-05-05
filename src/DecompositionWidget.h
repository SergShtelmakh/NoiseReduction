#ifndef DECOMPOSITIONWIDGET_H
#define DECOMPOSITIONWIDGET_H

#include <QWidget>

#include <src/audio/Audio.h>

class DecompositionItemWidget;

namespace Ui {
class DecompositionWidget;
}

class DecompositionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DecompositionWidget(QWidget *parent = 0);
    ~DecompositionWidget();

    void setDecomposition(const Audio::SignalsSourceVector &decomposiiton);
    Audio::SignalsSourceVector thresholdedSignals();

private:
    void clearWidget();

    Ui::DecompositionWidget *ui;
    QVector<DecompositionItemWidget *> m_widgets;
};

#endif // DECOMPOSITIONWIDGET_H
