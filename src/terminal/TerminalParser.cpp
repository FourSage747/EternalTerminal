#include "TerminalParser.h"
#include "TerminalBuffer.h" // Підключаємо щоб бачити методи буфера
#include <QDebug>

TerminalParser::TerminalParser(TerminalBuffer* buffer)
    : m_buffer(buffer)
{
}

QString TerminalParser::clean(QString input)
{
    return input;
}

void TerminalParser::parse(const QString& data)
{
    for(QChar c : data)
    {
        switch(parserState)
        {
            case ParserState::Normal:
            {
                if(c == '\x1b') {
                    parserState = ParserState::Escape;
                } else {
                    // Кажемо буферу просто вивести символ
                    m_buffer->processCharacter(c);
                }
                break;
            }
            case ParserState::Escape:
            {
                if(c == '[') {
                    parserState = ParserState::CSI;
                    csiBuffer.clear();
                } else if(c == ']') {
                    parserState = ParserState::OSC;
                    oscBuffer.clear();
                } else {
                    parserState = ParserState::Normal;
                }
                break;
            }
            case ParserState::CSI:
            {
                csiBuffer += c;
                if(c.isLetter() || c == '@') {
                    qDebug() << "CSI:" << csiBuffer;
                    processCSI(csiBuffer);
                    csiBuffer.clear();
                    parserState = ParserState::Normal;
                }
                break;
            }
            case ParserState::OSC:
            {
                if(c == '\a') {
                    parserState = ParserState::Normal;
                    oscBuffer.clear();
                } else {
                    oscBuffer += c;
                }
                break;
            }
        }
    }
}

void TerminalParser::processCSI(const QString& command)
{
    int value = 1;
    QString number = command.left(command.length() - 1);

    if(!number.isEmpty())
    {
        value = number.toInt();
        if(value <= 0) value = 1;
    }

    QChar type = command.back();

    if(type == 'A') m_buffer->moveCursorUp(value);
    else if(type == 'B') m_buffer->moveCursorDown(value);
    else if(type == 'C') m_buffer->moveCursorRight(value);
    else if(type == 'D') m_buffer->moveCursorLeft(value);
    else if(type == 'K') {
        int mode = 0;
        if(!number.isEmpty()) mode = number.toInt();
        m_buffer->clearLine(mode);
    }
    else if(type == 'P') m_buffer->deleteCharacters(value);
    else if(type == '@') m_buffer->insertBlankCharacters(value);
}