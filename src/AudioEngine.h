#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>

class AudioEngine : public QObject
{
    Q_OBJECT
public:
    explicit AudioEngine(QObject *parent = 0);

signals:

public slots:
};

#endif // AUDIOENGINE_H