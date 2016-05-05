#include "DecompositionWidget.h"
#include "ui_DecompositionWidget.h"

#include <src/DecompositionItemWidget.h>
#include <QScrollArea>

DecompositionWidget::DecompositionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecompositionWidget)
{
    ui->setupUi(this);
}

DecompositionWidget::~DecompositionWidget()
{
    delete ui;
}

void DecompositionWidget::setDecomposition(const Audio::SignalsSourceVector &decomposiiton)
{
    clearWidget();

    auto layout = new QVBoxLayout(this);
    ui->scrollAreaWidgetContents->setLayout(layout);
    for (auto item : decomposiiton) {
        auto wdg = new DecompositionItemWidget(this);
        m_widgets.push_back(wdg);
        wdg->setSignal(item);
        layout->addWidget(wdg);
    }
}

Audio::SignalsSourceVector DecompositionWidget::thresholdedSignals()
{
    Audio::SignalsSourceVector result;
    for (auto w : m_widgets) {
        result << w->thresholded();
    }

    return result;
}

void DecompositionWidget::clearWidget()
{
    while (!m_widgets.empty()) {
        auto w = m_widgets.takeFirst();
        w->deleteLater();
    }
}
