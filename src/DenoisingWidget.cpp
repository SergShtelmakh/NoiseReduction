#include "DenoisingWidget.h"
#include "ui_DenoisingWidget.h"

#include <src/DenoisingManager.h>
#include <src/ThresholdsWidget.h>
#include <src/ThresholdsManager.h>
#include <src/PlotManager.h>

#include <QScrollArea>
#include <QLayoutItem>

namespace {
    const bool cShowThresholdsWidgetSeparately = false;
}

DenoisingWidget::DenoisingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DenoisingWidget)
    , m_denoisingManager(new DenoisingManager)
{
    ui->setupUi(this);

    ui->cbWaveletType->addItems(Wavelet::waveletFunctionsNames());
    ui->cbThresholdType->addItems(ThresholdsManager::thresholdsNames());
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
        delete w;
    }
}

void DenoisingWidget::on_pbPrepare_clicked()
{
    clearWidget();

    QLayout *layout;
    if (cShowThresholdsWidgetSeparately) {
        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setBackgroundRole(QPalette::Window);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);

        QWidget* wdg = new QWidget();
        wdg->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        wdg->setLayout(new QVBoxLayout(wdg));
        scrollArea->setWidget(wdg);
        scrollArea->show();

        layout = wdg->layout();
    } else {
        if (!ui->scrollAreaWidgetContents->layout()) {
            ui->scrollAreaWidgetContents->setLayout(new QVBoxLayout());
        }

        layout = ui->scrollAreaWidgetContents->layout();
    }

    m_denoisingManager->setSignal(*m_inputAudioSignal.data());
    m_denoisingManager->prepareToDenoising(ui->cbWaveletType->currentText(), ui->sbLevel->value());

    PlotManager::plot(ui->inputTransformedSignalWidget, m_denoisingManager->transformedSignal());

    auto decomposition = m_denoisingManager->transformedDecomposition();

    m_itemsCount = decomposition.size();



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
    m_denoisingManager->denoising(ui->cbThresholdType->currentText(), thresholdsData());

    PlotManager::plot(ui->outputTransformedSignalWidget, m_denoisingManager->thresholdedSignal());
    ui->outputSignalPlayerWidget->setSignalSource(m_denoisingManager->outputSignal());
    PlotManager::createPlot(Audio::makeSignalDifference(m_inputAudioSignal->source(), m_denoisingManager->outputSignal()));
}
