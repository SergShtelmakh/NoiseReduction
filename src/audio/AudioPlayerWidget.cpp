#include "AudioPlayerWidget.h"
#include "ui_AudioPlayerWidget.h"

#include <QMediaPlayer>
#include <QTimer>

AudioPlayerWidget::AudioPlayerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AudioPlayerWidget)
    , m_signal(new AudioSignal)
    , m_player(new QMediaPlayer)
    , m_playerStatusChecker(new QTimer)
{
    ui->setupUi(this);

    ui->audioPlot->addGraph(); // signal
    ui->audioPlot->addGraph(); // time line
    ui->audioPlot->graph(1)->setPen(QPen(QBrush(Qt::red), 5));
    ui->audioPlot->xAxis->setLabel("Time");
    ui->audioPlot->yAxis->setLabel("Amplitude");

    m_playerStatusChecker->setInterval(100);

    connect(m_playerStatusChecker.data(), &QTimer::timeout, this, &AudioPlayerWidget::updateTimeLinePlot);
    connect(m_player.data(), &QMediaPlayer::stateChanged, this, &AudioPlayerWidget::updateButtonState);

    updateButtonState();
}

AudioPlayerWidget::~AudioPlayerWidget()
{
    delete ui;
}

void AudioPlayerWidget::setFileName(const QString &fileName)
{
    m_signal.reset(new AudioSignal(fileName));
    m_player->setMedia(QUrl::fromLocalFile(fileName));
    updateSignalPlot();
    updateTimeLinePlot();
}

void AudioPlayerWidget::setSignalSource(const Audio::SignalSource &signalSource)
{
    m_signal.reset(new AudioSignal(signalSource));
    auto fileName = Audio::generateAudioFileName();
    m_signal->save(fileName);
    m_player->setMedia(QUrl::fromLocalFile(fileName));
    updateSignalPlot();
    updateTimeLinePlot();
}

void AudioPlayerWidget::on_pbPlay_clicked()
{
    if (m_player->state() == QMediaPlayer::PlayingState) {
        return;
    }

    m_player->play();
    m_playerStatusChecker->start();
}

void AudioPlayerWidget::on_pbPause_clicked()
{
    if (m_player->state() != QMediaPlayer::PlayingState) {
        return;
    }

    m_player->pause();
    m_playerStatusChecker->stop();
}

void AudioPlayerWidget::on_pbStop_clicked()
{
    if (m_player->state() == QMediaPlayer::StoppedState) {
        return;
    }

    m_player->stop();
    m_playerStatusChecker->stop();

    updateTimeLinePlot();
}

void AudioPlayerWidget::updateButtonState()
{
    auto playerState = m_player->state();
    ui->pbPlay->setEnabled(playerState != QMediaPlayer::PlayingState);
    ui->pbStop->setEnabled(playerState != QMediaPlayer::StoppedState);
    ui->pbPause->setEnabled(playerState == QMediaPlayer::PlayingState);
}

void AudioPlayerWidget::updateSignalPlot()
{
    auto source = m_signal->source();
    auto size = source.size();
    if (size == 0) {
        return;
    }

    QVector<double> x;
    double maxX = size;
    double minX = 0;
    auto dx = (maxX - minX) / size;
    auto currentX = minX;
    double minY = source.first();
    double maxY = source.first();
    for (auto i = 0; i < size; ++i) {
        x.push_back(currentX);
        currentX += dx;
        minY = qMin(source[i], minY);
        maxY = qMax(source[i], maxY);
    }

    ui->audioPlot->xAxis->setRange(minX, maxX);
    ui->audioPlot->yAxis->setRange(minY, maxY);

    m_plotRanges = Ranges { minX, maxX, minY, maxY };

    ui->audioPlot->graph(0)->setData(x, m_signal->source());
    ui->audioPlot->replot();
}

void AudioPlayerWidget::updateTimeLinePlot()
{
    double currentTime = m_player->position();
    ui->lcdTime->display(currentTime / 1000.0);

    double xBase = m_plotRanges.maxX * currentTime / m_signal->audioLength();
    QVector<double> x,y;
    x << xBase << xBase + 1;
    y << m_plotRanges.minY << m_plotRanges.maxY;

    ui->audioPlot->graph(1)->setData(x, y);
    ui->audioPlot->replot();
}
