#include "DecompositionWidget.h"
#include "ui_DecompositionWidget.h"

#include <src/DecompositionItemWidget.h>
#include <QScrollArea>

DecompositionWidget::DecompositionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecompositionWidget)
{
    ui->setupUi(this);
    ui->inputSignalPlayerWidget->setFileName("test.wav");
}

DecompositionWidget::~DecompositionWidget()
{
    delete ui;
}

void DecompositionWidget::setDecomposition(const Audio::SignalsSourceVector &decomposition)
{
    clearWidget();

    m_itemsCount = decomposition.size();

    auto layout = new QVBoxLayout(this);
    ui->scrollAreaWidgetContents->setLayout(layout);
    for (auto item : decomposition) {
        auto wdg = new DecompositionItemWidget(this);
        m_widgets.push_back(wdg);
        wdg->setSignalSource(item);
        layout->addWidget(wdg);
    }

    Q_ASSERT(m_itemsCount == m_widgets.size());
}

QVector<double> DecompositionWidget::thresholdsData() const
{
    QVector<double> result;
    for (auto w : m_widgets) {
        result << w->threshold();
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
