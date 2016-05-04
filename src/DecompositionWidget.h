#ifndef DECOMPOSITIONWIDGET_H
#define DECOMPOSITIONWIDGET_H

#include <QWidget>

#include <src/Audio.h>

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

private:
    Ui::DecompositionWidget *ui;
    QVector<QWidget *> m_widgets;
};

#endif // DECOMPOSITIONWIDGET_H
