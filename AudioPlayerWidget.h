#ifndef AUDIOPLAYERWIDGET_H
#define AUDIOPLAYERWIDGET_H

#include <QWidget>

namespace Ui {
class AudioPlayerWidget;
}

class AudioPlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudioPlayerWidget(QWidget *parent = 0);
    ~AudioPlayerWidget();

private slots:
    void on_pbPlay_clicked();

    void on_pbPause_clicked();

    void on_pbStop_clicked();

private:
    Ui::AudioPlayerWidget *ui;
};

#endif // AUDIOPLAYERWIDGET_H
