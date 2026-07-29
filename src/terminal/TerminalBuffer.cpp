#include "TerminalBuffer.h"
#include "TerminalSelection.h"
#include <QDebug>


TerminalBuffer::TerminalBuffer(QObject *parent)
    : QObject(parent)
{

    resize(24,80);
    m_lastScrollbackSize = scrollback.size();


    updateTimer = new QTimer(this);

    updateTimer->setInterval(16);


    connect(
        updateTimer,
        &QTimer::timeout,
        this,
        [this]()
        {
            if(dirty)
            {
                dirty = false;

                emit screenChanged();
                emit cursorChanged();
            }
        }
    );


    updateTimer->start();

}



int TerminalBuffer::cursorX() const
{
    return m_cursorColumn;
}



int TerminalBuffer::cursorY() const
{
    return m_cursorRow;
}



int TerminalBuffer::rows() const
{
    return m_rows;
}



int TerminalBuffer::columns() const
{
    return m_columns;
}

int TerminalBuffer::scrollbackSize() const
{
    return scrollback.size();
}

int TerminalBuffer::screenCursorY() const
{
    return m_cursorRow;
}

int TerminalBuffer::totalRows() const
{
    return scrollback.size() + m_rows;
}

int TerminalBuffer::visibleOffset() const
{
    return scrollback.size();
}

int TerminalBuffer::scrollOffset() const
{
    return m_scrollOffset;
}


// -----------(Old)----------------
// QString TerminalBuffer::characterAt(
//     int row,
//     int column
// ) const
// {

//     if(
//         row < 0 ||
//         row >= m_rows ||
//         column < 0 ||
//         column >= m_columns
//     )
//     {
//         return " ";
//     }


//     return QString(
//         screen[row][column].character
//     );

// }

// ------Now work-----------
QString TerminalBuffer::characterAt(
    int row,
    int column
) const
{

    if(column < 0 || column >= m_columns)
    {
        return " ";
    }


    int historySize = scrollback.size();


    if(row < historySize)
    {
        if(row < scrollback.size())
        {
            return QString(
                scrollback[row][column].character
            );
        }

        return " ";
    }


    int screenRow = row - historySize;


    if(
        screenRow >= 0 &&
        screenRow < screen.size()
    )
    {
        return QString(
            screen[screenRow][column].character
        );
    }


    return " ";
}

// ---------New(not work)------------
// QString TerminalBuffer::characterAt(
//     int row,
//     int column
// ) const
// {

//     if(
//         column < 0 ||
//         column >= m_columns
//     )
//     {
//         return " ";
//     }


//     int visibleStart =
//         scrollback.size()
//         - m_scrollOffset
//         - m_rows;


//     int absoluteRow =
//         visibleStart + row;


//     if(absoluteRow < 0)
//     {
//         return " ";
//     }


//     if(absoluteRow < scrollback.size())
//     {
//         return QString(
//             scrollback[absoluteRow][column].character
//         );
//     }


//     int screenRow =
//         absoluteRow - scrollback.size();


//     if(
//         screenRow >= 0 &&
//         screenRow < screen.size()
//     )
//     {
//         return QString(
//             screen[screenRow][column].character
//         );
//     }


//     return " ";
// }

void TerminalBuffer::resize(
    int rows,
    int columns
)
{

    QVector<QVector<Cell>> oldScreen = screen;


    int oldRows = m_rows;
    int oldColumns = m_columns;


    m_rows = rows;
    m_columns = columns;


    screen.clear();


    for(int y = 0; y < rows; y++)
    {
        QVector<Cell> line;

        for(int x = 0; x < columns; x++)
        {
            line.append(Cell());
        }

        screen.append(line);
    }


    int copyRows = qMin(rows, oldRows);
    int copyColumns = qMin(columns, oldColumns);


    for(int y = 0; y < copyRows; y++)
    {
        if(y >= oldScreen.size())
            break;


        for(int x = 0; x < copyColumns; x++)
        {
            if(x >= oldScreen[y].size())
                break;


            screen[y][x] = oldScreen[y][x];
        }
    }


    if(m_cursorRow >= m_rows)
        m_cursorRow = m_rows - 1;


    if(m_cursorColumn >= m_columns)
        m_cursorColumn = m_columns - 1;


    emit screenChanged();
}


void TerminalBuffer::putCharacter(QChar c)
{

    if(
        m_cursorRow < 0 ||
        m_cursorRow >= m_rows ||
        m_cursorColumn < 0 ||
        m_cursorColumn >= m_columns
    )
    {
        return;
    }


    screen[m_cursorRow][m_cursorColumn].character = c;

    m_cursorColumn++;

    if(m_cursorColumn > m_lineEndColumn)
    {
        m_lineEndColumn = m_cursorColumn;
    }

    if(m_cursorColumn >= m_columns)
    {
        m_cursorColumn = 0;

        m_cursorRow++;


        if(m_cursorRow >= m_rows)
        {
            scrollback.append(screen.first());

            screen.removeFirst();


            QVector<Cell> newLine;

            for(int x = 0; x < m_columns; x++)
            {
                newLine.append(Cell());
            }


            screen.append(newLine);


            m_cursorRow = m_rows - 1;
        }
    }

}

void TerminalBuffer::processCharacter(QChar c)
{

    qDebug()
    << "CHAR:"
    << c
    << "CURSOR:"
    << m_cursorRow
    << m_cursorColumn;

    if(c == '\a')
    {
        return;
    }

    // if(c == '\n')
    // {
    //     m_cursorRow++;

    //     if(m_cursorRow >= m_rows)
    //     {
    //         m_cursorRow = m_rows - 1;
    //     }

    //     m_cursorColumn = 0;
    //     m_lineEndColumn = 0;

    //     return;
    // }
    if(c == '\n')
    {
        m_cursorRow++;

        if(m_cursorRow >= m_rows)
        {
            scrollback.append(screen.first());

            screen.removeFirst();


            QVector<Cell> newLine;

            for(int x = 0; x < m_columns; x++)
            {
                newLine.append(Cell());
            }


            screen.append(newLine);


            m_cursorRow = m_rows - 1;
        }


        m_cursorColumn = 0;
        m_lineEndColumn = 0;

        return;
    }


    if(c == '\r')
    {
        m_cursorColumn = 0;
        m_lineEndColumn = 0;
        return;
    }


    if(c == '\b')
    {
        if(m_cursorColumn > 0)
        {
            m_cursorColumn--;
        }

        return;
    }

    putCharacter(c);

    m_inputStartColumn = qMax(
        m_inputStartColumn,
        m_cursorColumn
    );

}



void TerminalBuffer::append(QString data)
{

    for(QChar c : data)
    {

        switch(parserState)
        {

            case ParserState::Normal:
            {

                if(c == '\x1b')
                {
                    parserState = ParserState::Escape;
                }
                else
                {
                    processCharacter(c);
                }

                break;
            }



            case ParserState::Escape:
            {

                if(c == '[')
                {
                    parserState = ParserState::CSI;

                    csiBuffer.clear();
                }
                else if(c == ']')
                {
                    parserState = ParserState::OSC;

                    oscBuffer.clear();
                }
                else
                {
                    parserState = ParserState::Normal;
                }

                break;
            }



           case ParserState::CSI:
            {

                csiBuffer += c;


                if(
                    c.isLetter() ||
                    c == '@'
                )
                {

                    qDebug()
                        << "CSI:"
                        << csiBuffer;


                    processCSI(csiBuffer);


                    csiBuffer.clear();


                    parserState = ParserState::Normal;

                }

                break;
            }



            case ParserState::OSC:
            {

                if(c == '\a')
                {
                    parserState = ParserState::Normal;
                    oscBuffer.clear();
                }
                else
                {
                    oscBuffer += c;
                }

                break;
            }

        }

    }

   if(m_followOutput)
    {
        m_scrollOffset = 0;
    }
    else
    {
        int delta =
            scrollback.size()
            - m_lastScrollbackSize;


        if(delta > 0)
        {
            m_scrollOffset += delta;
        }
    }


    m_lastScrollbackSize = scrollback.size();

    dirty = true;

}

void TerminalBuffer::processCSI(QString command)
{

    qDebug()
    << "CSI COMMAND:"
    << command
    << "CURSOR BEFORE:"
    << m_cursorRow
    << m_cursorColumn;
    
    int value = 1;


    QString number =
        command.left(command.length() - 1);


    if(!number.isEmpty())
    {
        value = number.toInt();

        if(value <= 0)
            value = 1;
    }


    QChar type =
        command.back();



    // Cursor Up
    if(type == 'A')
    {
        m_cursorRow -= value;


        if(m_cursorRow < 0)
            m_cursorRow = 0;


        return;
    }



    // Cursor Down
    if(type == 'B')
    {
        m_cursorRow += value;


        if(m_cursorRow >= m_rows)
            m_cursorRow = m_rows - 1;


        return;
    }



    // Cursor Right
    if(type == 'C')
    {
        m_cursorColumn += value;
        return;
    }


    if(type == 'D')
    {
        m_cursorColumn -= value;

        if(m_cursorColumn < 0)
            m_cursorColumn = 0;

        return;
    }



    // Clear line
    if(type == 'K')
    {

        int mode = 0;


        QString param =
            command.left(command.length()-1);


        if(!param.isEmpty())
            mode = param.toInt();



        if(mode == 2)
        {

            for(int x = 0; x < m_columns; x++)
            {
                screen[m_cursorRow][x].character = ' ';
            }

            m_cursorColumn = 0;

        }
        else
        {

            for(
                int x = m_cursorColumn;
                x < m_columns;
                x++
            )
            {
                screen[m_cursorRow][x].character = ' ';
            }

        }


        return;
    }



    // Delete characters
    if(type == 'P')
    {

        for(
            int x = m_cursorColumn;
            x < m_columns - value;
            x++
        )
        {
            screen[m_cursorRow][x].character =
                screen[m_cursorRow][x + value].character;
        }


        for(
            int x = m_columns - value;
            x < m_columns;
            x++
        )
        {
            screen[m_cursorRow][x].character = ' ';
        }


        return;
    }



    // Insert blank characters
    if(type == '@')
    {

        int count = value;


        for(
            int x = m_columns - 1;
            x >= m_cursorColumn + count;
            x--
        )
        {
            screen[m_cursorRow][x].character =
                screen[m_cursorRow][x-count].character;
        }


        for(
            int x = 0;
            x < count;
            x++
        )
        {
            screen[m_cursorRow][m_cursorColumn+x].character = ' ';
        }


        return;
    }

}

void TerminalBuffer::reflowScreen(int oldColumns)
{
    if(oldColumns == m_columns)
        return;


    QVector<QVector<Cell>> old = screen;


    screen.clear();


    QVector<Cell> currentLine;


    for(const auto &line : old)
    {

        int lastChar = line.size() - 1;


        while(
            lastChar >= 0 &&
            line[lastChar].character == ' '
        )
        {
            lastChar--;
        }


        for(int x = 0; x <= lastChar; x++)
        {

            currentLine.append(line[x]);


            if(currentLine.size() >= m_columns)
            {
                screen.append(currentLine);

                currentLine.clear();
            }
        }
    }


    if(!currentLine.isEmpty())
    {
        while(currentLine.size() < m_columns)
        {
            currentLine.append(Cell());
        }

        screen.append(currentLine);
    }


    while(screen.size() < m_rows)
    {
        QVector<Cell> empty;

        for(int x = 0; x < m_columns; x++)
        {
            empty.append(Cell());
        }

        screen.prepend(empty);
    }


    while(screen.size() > m_rows)
    {
        scrollback.append(screen.first());
        screen.removeFirst();
    }


    m_cursorRow = qBound(
        0,
        m_cursorRow,
        m_rows - 1
    );


    m_cursorColumn = qBound(
        0,
        m_cursorColumn,
        m_columns - 1
    );


    emit screenChanged();
}

// ---------Work version-----------
void TerminalBuffer::resizeTerminal(
    int rows,
    int columns
)
{

    if(
        rows == m_rows &&
        columns == m_columns
    )
    {
        return;
    }


    resize(rows, columns);


    emit resized(
        rows,
        columns
    );

}


// --------New (not work)---------
// void TerminalBuffer::resizeTerminal(
//     int rows,
//     int columns
// )
// {

//     if(
//         rows == m_rows &&
//         columns == m_columns
//     )
//     {
//         return;
//     }


//     QVector<QVector<Cell>> oldScreen = screen;


//     m_rows = rows;
//     m_columns = columns;


//     screen.clear();


//     QVector<Cell> current;


//     for(const auto &oldLine : oldScreen)
//     {
//         int last = oldLine.size() - 1;


//         while(
//             last >= 0 &&
//             oldLine[last].character == ' '
//         )
//         {
//             last--;
//         }


//         for(int x = 0; x <= last; x++)
//         {
//             current.append(oldLine[x]);


//             if(current.size() == m_columns)
//             {
//                 screen.append(current);
//                 current.clear();
//             }
//         }
//     }


//     if(!current.isEmpty())
//     {
//         while(current.size() < m_columns)
//         {
//             current.append(Cell());
//         }

//         screen.append(current);
//     }


//     while(screen.size() < m_rows)
//     {
//         QVector<Cell> empty;

//         for(int x = 0; x < m_columns; x++)
//         {
//             empty.append(Cell());
//         }

//         screen.prepend(empty);
//     }


//     while(screen.size() > m_rows)
//     {
//         scrollback.append(screen.first());
//         screen.removeFirst();
//     }


//     m_cursorRow = qBound(
//         0,
//         m_cursorRow,
//         m_rows - 1
//     );


//     m_cursorColumn = qBound(
//         0,
//         m_cursorColumn,
//         m_columns - 1
//     );


//     emit screenChanged();

//     emit resized(
//         rows,
//         columns
//     );

// }

void TerminalBuffer::scroll(int amount)
{
    int maxOffset = scrollback.size();


    m_scrollOffset += amount;


    if(m_scrollOffset < 0)
        m_scrollOffset = 0;


    if(m_scrollOffset > maxOffset)
        m_scrollOffset = maxOffset;


    m_followOutput = (m_scrollOffset == 0);
    qDebug() << "Follow Output:" << m_followOutput;
    dirty = true;

    emit scrollChanged();
}

QVariantList TerminalBuffer::wordAt(
    int row,
    int column
) const
{

    QVariantList result;


    if(
        row < 0 ||
        row >= totalRows() ||
        column < 0 ||
        column >= m_columns
    )
    {
        return result;
    }


    QString line;


    for(int x = 0; x < m_columns; x++)
    {
        line += characterAt(row,x);
    }


    if(line[column].isSpace())
    {
        return result;
    }


    int start = column;

    while(
        start > 0 &&
        !line[start-1].isSpace()
    )
    {
        start--;
    }


    int end = column;

    while(
        end < m_columns-1 &&
        !line[end+1].isSpace()
    )
    {
        end++;
    }


    result.append(row);
    result.append(start);

    result.append(row);
    result.append(end);


    return result;
}
////=====Work version======
// QString TerminalBuffer::selectedText(
//     TerminalSelection *selection
// ) const
// {

//     if(!selection)
//         return "";


//     int startRow = selection->startRow();
//     int startColumn = selection->startColumn();

//     int endRow = selection->endRow();
//     int endColumn = selection->endColumn();



//     if(
//         startRow > endRow ||
//         (
//             startRow == endRow &&
//             startColumn > endColumn
//         )
//     )
//     {
//         std::swap(startRow,endRow);
//         std::swap(startColumn,endColumn);
//     }



//     QString result;



//     for(
//         int row = startRow;
//         row <= endRow;
//         row++
//     )
//     {

//         int from = 0;
//         int to = m_columns - 1;


//         if(row == startRow)
//             from = startColumn;


//         if(row == endRow)
//             to = endColumn;



//         for(
//             int col = from;
//             col <= to;
//             col++
//         )
//         {
//             result += screen[row][col].character;
//         }



//         if(row != endRow)
//             result += "\n";
//     }



//     return result.trimmed();
// }

///=====New version(Work)=====
QString TerminalBuffer::selectedText(
    TerminalSelection *selection
) const
{

    if(!selection)
        return "";


    int startRow = selection->startRow();
    int startColumn = selection->startColumn();

    int endRow = selection->endRow();
    int endColumn = selection->endColumn();



    if(
        startRow > endRow ||
        (
            startRow == endRow &&
            startColumn > endColumn
        )
    )
    {
        std::swap(startRow,endRow);
        std::swap(startColumn,endColumn);
    }



    QString result;


    int historySize = scrollback.size();

    for(
        int row = startRow;
        row <= endRow;
        row++
    )
    {

        int bufferRow = row;



        int from = 0;
        int to = m_columns - 1;


        if(row == startRow)
            from = startColumn;


        if(row == endRow)
            to = endColumn;



        for(
            int col = from;
            col <= to;
            col++
        )
        {

            if(bufferRow < historySize)
            {
                result +=
                    scrollback[bufferRow][col].character;
            }
            else
            {
                int screenRow =
                    bufferRow - historySize;


                if(
                    screenRow >= 0 &&
                    screenRow < screen.size()
                )
                {
                    result +=
                        screen[screenRow][col].character;
                }
            }

        }



        if(row != endRow)
            result += "\n";
    }



    return result.trimmed();
}
int TerminalBuffer::bufferRowFromScreenRow(
    int screenRow
) const
{
    int historySize = scrollback.size();

    int offset =
        historySize - m_scrollOffset;

    return offset + screenRow;
}