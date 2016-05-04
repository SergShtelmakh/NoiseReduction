#include "DecompositionWidget.h"
#include "ui_DecompositionWidget.h"

#include <src/DecompositionItem.h>
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

void DecompositionWidget::setDecomposition(const Audio::SignalsVectorQt &decomposiiton)
{
    auto layout = new QVBoxLayout(this);
    ui->scrollAreaWidgetContents->setLayout(layout);
    for (auto item : decomposiiton) {
        auto wdg = new DecompositionItem(this);
        m_widgets.push_back(wdg);
        wdg->setSignal(item);
        layout->addWidget(wdg);
    }
}

Audio::SignalsVectorQt DecompositionWidget::thresholdedSignals()
{
    Audio::SignalsVectorQt result;
    for (auto w : m_widgets) {
        result << w->thresholded();
    }

    return result;
}
