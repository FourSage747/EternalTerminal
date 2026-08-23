#include "PtyProcess.h"

#include <pty.h>
#include <unistd.h>

#include <QDebug>
#include <QSocketNotifier>
#include <termios.h>
#include <sys/ioctl.h>


PtyProcess::PtyProcess(QObject *parent)
    : QObject(parent)
{

    flushTimer = new QTimer(this);

    flushTimer->setInterval(16);


    connect(
        flushTimer,
        &QTimer::timeout,
        this,
        &PtyProcess::flush
    );


    flushTimer->start();

}



bool PtyProcess::start()
{

    pid_t pid;


    pid = forkpty(
        &masterFd,
        nullptr,
        nullptr,
        nullptr
    );


    if(pid == -1)
    {
        qDebug()
            << "forkpty failed";

        return false;
    }

    struct termios term;


    tcgetattr(
        masterFd,
        &term
    );


    tcsetattr(
        masterFd,
        TCSANOW,
        &term
    );



    if(pid == 0)
    {
        // Кажемо програмам, що ми підтримуємо стандарт xterm та 256 кольорів
        setenv("TERM", "xterm-256color", 1);

        execl(
            "/bin/bash",
            "bash",
            nullptr
        );


        _exit(1);

    }



    qDebug()
        << "PTY started with pid:"
        << pid;



    QSocketNotifier *notifier =
        new QSocketNotifier(
            masterFd,
            QSocketNotifier::Read,
            this
        );



    connect(
        notifier,
        &QSocketNotifier::activated,
        this,
        [this]()
        {

            char buffer[4096];


            int size =
                read(
                    masterFd,
                    buffer,
                    sizeof(buffer)-1
                );


            if(size > 0)
            {

                buffer[size] = '\0';


                pendingData += QString::fromUtf8(buffer);

            }

        }
    );


    return true;

}

void PtyProcess::write(QString data)
{

    QByteArray bytes = data.toUtf8();

    qDebug()
        << "PTY WRITE:"
        << bytes.toHex();

    ::write(
        masterFd,
        bytes.constData(),
        bytes.size()
    );

}

void PtyProcess::flush()
{

    if(pendingData.isEmpty())
    {
        return;
    }


    QString data = pendingData;


    pendingData.clear();


    emit dataReceived(data);

}

void PtyProcess::resize(int rows, int columns)
{

    if(masterFd < 0)
    {
        return;
    }


    struct winsize ws;

    ws.ws_row = rows;
    ws.ws_col = columns;

    ws.ws_xpixel = 0;
    ws.ws_ypixel = 0;


    ioctl(
        masterFd,
        TIOCSWINSZ,
        &ws
    );

}