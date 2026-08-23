#pragma once
#include <QString>

class TerminalBuffer; // Попереднє оголошення класу

class TerminalParser
{
public:
    // Конструктор тепер приймає вказівник на буфер
    explicit TerminalParser(TerminalBuffer* buffer);

    static QString clean(QString input);
    
    // Головний метод парсингу (сюди переїде логіка з append)
    void parse(const QString& data);

private:
    TerminalBuffer* m_buffer;

    // Перенесли з TerminalBuffer.h
    enum class ParserState
    {
        Normal,
        Escape,
        CSI,
        OSC,
        Charset
    };

    ParserState parserState = ParserState::Normal;
    QString csiBuffer;
    QString oscBuffer;

    void processCSI(const QString& command);
};