#pragma once

#include <QObject>

#include "PtyProcess.h"
#include "TerminalBuffer.h"

#include "TerminalSelection.h"

class TerminalEngine : public QObject
{
    Q_OBJECT

    Q_PROPERTY(TerminalBuffer* terminalBuffer READ buffer CONSTANT)

    Q_PROPERTY(
        TerminalSelection* selection
        READ selection
        CONSTANT
    )

public:

    explicit TerminalEngine(QObject *parent = nullptr);


    Q_INVOKABLE void startShell();

    Q_INVOKABLE void sendInput(QString input);

    TerminalBuffer* buffer();
    TerminalSelection* selection();

signals:

    void outputReceived(QString output);



private:

    PtyProcess *pty;

    TerminalBuffer terminalBuffer;
    TerminalSelection terminalSelection;
};
