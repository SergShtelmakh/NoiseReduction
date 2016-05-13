#include "AudioPlayerWidget.h"
#include "ui_AudioPlayerWidget.h"

AudioPlayerWidget::AudioPlayerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AudioPlayerWidget)
    , m_signal(new AudioSignal)
{
    ui->setupUi(this);

    ui->audioPlot->addGraph(); // signal
    ui->audioPlot->addGraph(); // time line
    ui->audioPlot->xAxis->setLabel("Time");
    ui->audioPlot->yAxis->setLabel("Amplitude");

    m_playerStatusChecker.setInterval(100);

    connect(&m_playerStatusChecker, &QTimer::timeout, this, &AudioPlayerWidget::timerTimeout);
}

AudioPlayerWidget::~AudioPlayerWidget()
{
    delete ui;
}

void AudioPlayerWidget::setFileName(const QString &fileName)
{
    m_signal.reset(new AudioSignal(fileName));
    m_player.setMedia(QUrl::fromLocalFile(fileName));
    updateSignalPlot();
    updateTimeLinePlot();
}

void AudioPlayerWidget::setSignalSource(const Audio::SignalSource &signalSource)
{
    m_signal.reset(new AudioSignal(signalSource));
    auto fileName = "temp.wav";
    m_signal->save(fileName);
    m_player.setMedia(QUrl::fromLocalFile(fileName));
    updateSignalPlot();
    updateTimeLinePlot();
}

void AudioPlayerWidget::on_pbPlay_clicked()
{
    if (m_player.state() == QMediaPlayer::PlayingState) {
        return;
    }

    m_player.play();
    m_playerStatusChecker.start();
}

void AudioPlayerWidget::on_pbPause_clicked()
{
    if (m_player.state() != QMediaPlayer::PlayingState) {
        return;
    }

    m_player.pause();
    m_playerStatusChecker.stop();
}

void AudioPlayerWidget::on_pbStop_clicked()
{
    if (m_player.state() == QMediaPlayer::StoppedState) {
        return;
    }

    m_player.stop();
    m_playerStatusChecker.stop();
}

void AudioPlayerWidget::timerTimeout()
{
    m_currentTime = m_player.position();
    ui->lcdTime->display(m_currentTime / 1000.0);
    updateTimeLinePlot();
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
    double xBase = m_plotRanges.maxX * m_currentTime / m_signal->audioLength();

    QVector<double> x,y;
    x << xBase << xBase + 1;
    y << m_plotRanges.minY << m_plotRanges.maxY;

    ui->audioPlot->graph(1)->setData(x, y);
    ui->audioPlot->replot();
}
