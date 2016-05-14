#include "DenoisingWidget.h"
#include "ui_DenoisingWidget.h"

#include <src/DecompositionItemWidget.h>
#include <src/PlotManager.h>

#include <QScrollArea>

namespace
{
    const QString cTestFileName = "test.wav";
}


DenoisingWidget::DenoisingWidget(bool noised, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DenoisingWidget)
    , m_denoisingManager(new DenoisingManager)
{
    ui->setupUi(this);

    // TODO custom file name
    m_audioSignal.reset(new AudioSignal(cTestFileName));

    if (noised)
        m_audioSignal->makeWhiteNoise();

    ui->inputSignalPlayerWidget->setSignalSource(m_audioSignal->source());
    m_denoisingManager->setSignal(*m_audioSignal.data());

    processAudioSignal();
}

DenoisingWidget::~DenoisingWidget()
{
    delete ui;
}

QVector<double> DenoisingWidget::thresholdsData() const
{
    QVector<double> result;
    for (auto w : m_widgets) {
        result << w->thresholdValue();
    }

    return result;
}

void DenoisingWidget::clearWidget()
{
    while (!m_widgets.empty()) {
        auto w = m_widgets.takeFirst();
        w->deleteLater();
    }
}

void DenoisingWidget::processAudioSignal()
{
    m_denoisingManager->makeTransform();

    auto decomposition = m_denoisingManager->transformedDecomposition();

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

void DenoisingWidget::on_pbProcess_clicked()
{
    m_denoisingManager->makeThreshold(thresholdsData());
    m_denoisingManager->makeInverseTransform();

    ui->outputSignalPlayerWidget->setSignalSource(m_denoisingManager->outputSignal());

    PlotManager::plot(m_denoisingManager->thresholdedSignal());
}

