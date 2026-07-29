#include "TerminalEngine.h"

#include <QDebug>
#include <QGuiApplication>
#include <QClipboard>



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
    }

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

void TerminalEngine::copySelection()
{

    QString text =
        terminalBuffer.selectedText(
            selection()
        );


    if(text.isEmpty())
        return;


    QClipboard *clipboard =
        QGuiApplication::clipboard();


    clipboard->setText(text);

}
void TerminalEngine::paste()
{

    QClipboard *clipboard =
        QGuiApplication::clipboard();


    QString text =
        clipboard->text();


    if(text.isEmpty())
        return;


    sendInput(text);

}