#include "DenoisingWidget.h"
#include "ui_DenoisingWidget.h"

#include <src/ThresholdsWidget.h>
#include <src/PlotManager.h>

#include <QScrollArea>

DenoisingWidget::DenoisingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DenoisingWidget)
    , m_denoisingManager(new DenoisingManager)
{
    ui->setupUi(this);

    ui->cbWaveletType->addItems(Wavelet::waveletFunctionsNames());
}

DenoisingWidget::~DenoisingWidget()
{
    delete ui;
}

void DenoisingWidget::setSignal(const AudioSignal &signal)
{
    m_inputAudioSignal.reset(new AudioSignal(signal.source()));
    ui->inputSignalPlayerWidget->setSignalSource(m_inputAudioSignal->source());

    m_outputAudioSignal.reset(new AudioSignal(signal.source()));
    ui->outputSignalPlayerWidget->setSignalSource(m_outputAudioSignal->source());
}

QVector<double> DenoisingWidget::thresholdsData() const
{
    QVector<double> result;
    for (auto w : m_widgets) {
        result << w->threshold();
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

void DenoisingWidget::on_pbPrepare_clicked()
{
    clearWidget();

    m_denoisingManager->setSignal(*m_inputAudioSignal.data());
    m_denoisingManager->setWaveletName(ui->cbWaveletType->currentText());
    m_denoisingManager->setLevel(ui->sbLevel->value());
    m_denoisingManager->makeTransform();
    PlotManager::plot(ui->inputTransformedSignalWidget, m_denoisingManager->transformedSignal());

    auto decomposition = m_denoisingManager->transformedDecomposition();

    m_itemsCount = decomposition.size();

    if (!ui->scrollAreaWidgetContents->layout()) {
        ui->scrollAreaWidgetContents->setLayout(new QVBoxLayout(this));
    }

    auto layout = ui->scrollAreaWidgetContents->layout();
    for (auto item : decomposition) {
        auto wdg = new ThresholdsWidget(this);
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

    PlotManager::plot(ui->outputTransformedSignalWidget, m_denoisingManager->transformedSignal());
    ui->outputSignalPlayerWidget->setSignalSource(m_denoisingManager->outputSignal());
    PlotManager::plot(Audio::makeSignalDifference(m_inputAudioSignal->source(), m_denoisingManager->outputSignal()));
}
