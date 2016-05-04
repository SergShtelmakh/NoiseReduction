#pragma once

#include <QObject>

class AudioEngine : public QObject
{
    Q_OBJECT
public:
    explicit AudioEngine(QObject *parent = 0);
};
