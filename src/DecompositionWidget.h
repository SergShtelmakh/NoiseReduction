#ifndef DECOMPOSITIONWIDGET_H
#define DECOMPOSITIONWIDGET_H

#include <QWidget>

#include <src/audio/Audio.h>

class DecompositionItem;

namespace Ui {
class DecompositionWidget;
}

class DecompositionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DecompositionWidget(QWidget *parent = 0);
    ~DecompositionWidget();

    void setDecomposition(const Audio::SignalsVectorQt &decomposiiton);
    Audio::SignalsVectorQt thresholdedSignals();

private:
    Ui::DecompositionWidget *ui;
    QVector<DecompositionItem *> m_widgets;
};

#endif // DECOMPOSITIONWIDGET_H
