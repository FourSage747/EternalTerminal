#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include "terminal/TerminalEngine.h"
#include "terminal/TerminalBuffer.h"
#include "ui/TerminalRenderer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setDesktopFileName("EternalTerminal");
    qmlRegisterType<TerminalRenderer>(
        "EternalTerminal",
        1,
        0,
        "TerminalRenderer"
    );

    qmlRegisterType<TerminalBuffer>(
        "EternalTerminal",
        1,
        0,
        "TerminalBuffer"
    );


    QQmlApplicationEngine engine;

    TerminalEngine terminalEngine;

    engine.rootContext()->setContextProperty(
        "terminalEngine",
        &terminalEngine
    );


    engine.loadFromModule(
        "EternalTerminal",
        "Main"
    );


    if(engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
