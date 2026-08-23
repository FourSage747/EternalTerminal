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
                } else if(c == '(' || c == ')') {
                    // Команда вибору набору символів (наприклад \x1b(B)
                    // Переходимо в новий стан, щоб безпечно проковтнути наступну букву
                    parserState = ParserState::Charset;
                } else if(c == 'M') { // <-- НОВА КОМАНДА (Reverse Index)
                    m_buffer->reverseIndex();
                    parserState = ParserState::Normal;
                } else {
                    parserState = ParserState::Normal;
                }
                break;
            }
            case ParserState::Charset:
            {
                // Ми знаходимось тут, коли приходить буква 'B' після '\x1b('
                // Нічого не робимо, просто повертаємось у нормальний режим
                parserState = ParserState::Normal;
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
    QChar type = command.back();
    QString paramsStr = command.left(command.length() - 1);

    // --- НОВЕ: Перевіряємо, чи це "приватна" команда (наприклад ?1049h) ---
    bool isPrivate = false;
    if (paramsStr.startsWith('?')) {
        isPrivate = true;
        paramsStr = paramsStr.mid(1); // Відрізаємо '?' для коректного парсингу чисел
    }
    
    // Розбиваємо параметри, якщо їх декілька (наприклад, "24;10")
    QStringList params = paramsStr.split(';', Qt::SkipEmptyParts);

    // Зчитуємо перший параметр (за замовчуванням 1)
    int p1 = params.size() > 0 ? params[0].toInt() : 1;
    if(p1 <= 0) p1 = 1; // Більшість команд ANSI використовують 1 як мінімум

    // Зчитуємо другий параметр (за замовчуванням 1)
    int p2 = params.size() > 1 ? params[1].toInt() : 1;
    if(p2 <= 0) p2 = 1;

    // --- НОВЕ: ОБРОБКА ПРИВАТНИХ РЕЖИМІВ (АЛЬТЕРНАТИВНИЙ ЕКРАН) ---
    if (isPrivate) {
        // 'h' - Увімкнути режим (Set Mode)
        // 1049 та 47 - це стандартні коди для Alternate Screen
        if (type == 'h' && (p1 == 1049 || p1 == 47)) {
            m_buffer->enableAlternateScreen();
        } 
        // 'l' - Вимкнути режим (Reset Mode)
        else if (type == 'l' && (p1 == 1049 || p1 == 47)) {
            m_buffer->disableAlternateScreen();
        }
        return; // Обов'язково виходимо, щоб приватні команди не плуталися зі стандартними
    }

    // --- СТАНДАРТНІ КОМАНДИ ---
    if(type == 'A') m_buffer->moveCursorUp(p1);
    else if(type == 'B') m_buffer->moveCursorDown(p1);
    else if(type == 'C') m_buffer->moveCursorRight(p1);
    else if(type == 'D') m_buffer->moveCursorLeft(p1);
    else if(type == 'K') {
        int mode = params.size() > 0 ? params[0].toInt() : 0;
        m_buffer->clearLine(mode);
    }
    else if(type == 'P') m_buffer->deleteCharacters(p1);
    else if(type == '@') m_buffer->insertBlankCharacters(p1);
    
    // --- НОВІ КОМАНДИ ДЛЯ NANO ТА VIM ---
    
    // 'H' або 'f' - Абсолютне позиціонування курсора (CUP)
    else if(type == 'H' || type == 'f') {
        // ANSI-координати починаються з 1, а наші масиви в C++ з 0, тому віднімаємо 1
        m_buffer->setCursorPosition(p1 - 1, p2 - 1);
    }
    // 'J' - Очищення екрана (ED)
    else if(type == 'J') {
        int mode = params.size() > 0 ? params[0].toInt() : 0;
        m_buffer->clearScreen(mode);
    }
    // 'd' - Vertical Line Position Absolute (VPA) - перехід на конкретний рядок
    else if(type == 'd') {
        m_buffer->setCursorPosition(p1 - 1, m_buffer->cursorX());
    }
    // 'G' - Cursor Character Absolute (CHA) - перехід на конкретну колонку
    else if(type == 'G') {
        m_buffer->setCursorPosition(m_buffer->cursorY(), p1 - 1);
    }
    // 'L' - Insert Lines (IL) - Вставити порожні рядки
    else if(type == 'L') {
        m_buffer->insertLines(p1);
    }
    // 'M' - Delete Lines (DL) - Видалити рядки (текст знизу підніметься)
    else if(type == 'M') {
        m_buffer->deleteLines(p1);
    }
    // 'r' - Set Scrolling Region (DECSTBM)
    else if(type == 'r') {
        int top = params.size() > 0 ? params[0].toInt() : 1;
        // Якщо другий параметр не заданий, беремо висоту екрана
        int bottom = params.size() > 1 ? params[1].toInt() : m_buffer->rows();
        m_buffer->setScrollRegion(top - 1, bottom - 1);
    }
}