#include "DecompositionWidget.h"
#include "ui_DecompositionWidget.h"

#include <src/DecompositionItemWidget.h>
#include <QScrollArea>

DecompositionWidget::DecompositionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DecompositionWidget)
    , m_denoisingManager(new DenoisingManager)
{
    ui->setupUi(this);
    ui->inputSignalPlayerWidget->setFileName("test.wav");
    m_audioSignal.reset(new AudioSignal());
    m_audioSignal->load("test.wav");
    m_denoisingManager->setSignal(*m_audioSignal.data());
    m_denoisingManager->makeManualDenoise();
    setDecomposition(m_denoisingManager->transformedDecomposition());
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
        result << w->thresholdValue();
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

void DecompositionWidget::on_pbProcess_clicked()
{
    m_denoisingManager->makeThreshold(thresholdsData());
    m_denoisingManager->makeInverseTransform();

    AudioSignal signal;
    signal.setSignalSource(m_denoisingManager->outputSignal());
    signal.save("temp.wave");

    ui->outputSignalPlayerWidget->setFileName("temp.wave");

}

