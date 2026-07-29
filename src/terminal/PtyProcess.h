#pragma once

#include <QObject>
#include <QTimer>


class PtyProcess : public QObject
{
    Q_OBJECT

public:

    explicit PtyProcess(QObject *parent = nullptr);


    bool start();

    void write(QString data);

    void resize(int rows, int columns);

signals:

    void dataReceived(QString data);


private slots:

    void flush();


private:

    int masterFd = -1;


    QString pendingData;


    QTimer *flushTimer = nullptr;

};
