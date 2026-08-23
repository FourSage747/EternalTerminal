#include "TerminalEngine.h"

#include <QDebug>



TerminalEngine::TerminalEngine(QObject *parent)
    : QObject(parent)
{

    pty = new PtyProcess(this);

    connect(
        pty,
        &PtyProcess::dataReceived,
        this,
        [this](QString data)
        {

            terminalBuffer.append(data);


            emit outputReceived(data);

        }
    );

    connect(
        &terminalBuffer,
        &TerminalBuffer::resized,
        this,
        [this](int rows, int columns)
        {
            pty->resize(rows, columns);
        }
    );

}



void TerminalEngine::startShell()
{

    qDebug()
        << "Starting PTY shell";


    if(!pty->start())
    {
        qDebug()
            << "Failed to start PTY";
        return;
    }

    // НОВИЙ РЯДОК: Передаємо реальні розміри буфера одразу при старті
    pty->resize(terminalBuffer.rows(), terminalBuffer.columns());
}

void TerminalEngine::sendInput(QString input)
{
    pty->write(input);
}
TerminalBuffer* TerminalEngine::buffer()
{
    return &terminalBuffer;
}
TerminalSelection* TerminalEngine::selection()
{
    return &terminalSelection;
}