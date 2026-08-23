#include "TerminalBuffer.h"
#include "TerminalParser.h"
#include <QDebug>


TerminalBuffer::TerminalBuffer(QObject *parent)
    : QObject(parent)
{

    m_parser = new TerminalParser(this);
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

qreal TerminalBuffer::scrollOffset() const
{
    return m_scrollOffset;
}

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

QChar TerminalBuffer::rawCharAt(int row, int column) const
{
    if(column < 0 || column >= m_columns) return ' ';

    int historySize = scrollback.size();

    if(row < historySize) {
        if(row < scrollback.size()) return scrollback[row][column].character;
        return ' ';
    }

    int screenRow = row - historySize;

    if(screenRow >= 0 && screenRow < screen.size()) {
        return screen[screenRow][column].character;
    }

    return ' ';
}

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

    // Скидаємо межі скролінгу на весь екран
    m_marginTop = 0;
    m_marginBottom = rows - 1;


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

    if(c == '\n')
    {
        // Якщо ми дійшли до нижньої межі скролінгу
        if (m_cursorRow == m_marginBottom) {
            // Якщо це звичайний термінал (на весь екран) - зберігаємо історію
            if (m_marginTop == 0 && m_marginBottom == m_rows - 1) {
                scrollback.append(screen.first());
                screen.removeFirst();
                QVector<Cell> newLine(m_columns);
                screen.append(newLine);
            } else {
                // Якщо це nano (є регіон) - просто зсуваємо текст всередині регіону
                screen.removeAt(m_marginTop);
                QVector<Cell> emptyLine(m_columns);
                screen.insert(m_marginBottom, emptyLine);
            }
        } else {
            m_cursorRow++;
            if(m_cursorRow >= m_rows) m_cursorRow = m_rows - 1;
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
    m_parser->parse(data);

    if(m_followOutput)
    {
        m_scrollOffset = 0;
    }
    else
    {
        int delta = scrollback.size() - m_lastScrollbackSize;
        if(delta > 0)
        {
            m_scrollOffset += delta;
        }
    }

    m_lastScrollbackSize = scrollback.size();
    dirty = true;
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

void TerminalBuffer::scroll(qreal amount)
{
    qreal maxOffset = scrollback.size();

    m_scrollOffset += amount;

    if(m_scrollOffset < 0.0)
        m_scrollOffset = 0.0;

    if(m_scrollOffset > maxOffset)
        m_scrollOffset = maxOffset;

    // Оскільки це дробові числа, порівнюємо з невеликою похибкою
    m_followOutput = (m_scrollOffset <= 0.001);
    
    emit scrollChanged();
}

int TerminalBuffer::bufferRowFromScreenRow(int screenRow) const
{
    int historySize = scrollback.size();
    
    // Відкидаємо дробові пікселі, щоб правильно клікати мишкою
    int offset = historySize - static_cast<int>(m_scrollOffset); 

    return offset + screenRow;
}

void TerminalBuffer::moveCursorUp(int value) {
    m_cursorRow -= value;
    if(m_cursorRow < 0) m_cursorRow = 0;
}

void TerminalBuffer::moveCursorDown(int value) {
    m_cursorRow += value;
    if(m_cursorRow >= m_rows) m_cursorRow = m_rows - 1;
}

void TerminalBuffer::moveCursorRight(int value) {
    m_cursorColumn += value;
}

void TerminalBuffer::moveCursorLeft(int value) {
    m_cursorColumn -= value;
    if(m_cursorColumn < 0) m_cursorColumn = 0;
}

void TerminalBuffer::clearLine(int mode) {
    if(mode == 2) {
        for(int x = 0; x < m_columns; x++) {
            screen[m_cursorRow][x].character = ' ';
        }
        m_cursorColumn = 0;
    } else {
        for(int x = m_cursorColumn; x < m_columns; x++) {
            screen[m_cursorRow][x].character = ' ';
        }
    }
}

void TerminalBuffer::deleteCharacters(int value) {
    for(int x = m_cursorColumn; x < m_columns - value; x++) {
        screen[m_cursorRow][x].character = screen[m_cursorRow][x + value].character;
    }
    for(int x = m_columns - value; x < m_columns; x++) {
        screen[m_cursorRow][x].character = ' ';
    }
}

void TerminalBuffer::insertBlankCharacters(int value) {
    for(int x = m_columns - 1; x >= m_cursorColumn + value; x--) {
        screen[m_cursorRow][x].character = screen[m_cursorRow][x - value].character;
    }
    for(int x = 0; x < value; x++) {
        screen[m_cursorRow][m_cursorColumn + x].character = ' ';
    }
}

void TerminalBuffer::setCursorPosition(int row, int column)
{
    m_cursorRow = row;
    if(m_cursorRow < 0) m_cursorRow = 0;
    if(m_cursorRow >= m_rows) m_cursorRow = m_rows - 1;

    m_cursorColumn = column;
    if(m_cursorColumn < 0) m_cursorColumn = 0;
    if(m_cursorColumn >= m_columns) m_cursorColumn = m_columns - 1;
}

void TerminalBuffer::clearScreen(int mode)
{
    // mode 2: Очистити весь екран і повернути курсор в (0,0) (використовує nano/vim при старті)
    if(mode == 2) {
        for(int r = 0; r < m_rows; r++) {
            for(int c = 0; c < m_columns; c++) {
                screen[r][c].character = ' ';
            }
        }
        m_cursorRow = 0;
        m_cursorColumn = 0;
    }
    // mode 0: Від курсора до кінця екрана
    else if (mode == 0) {
        clearLine(0);
        for(int r = m_cursorRow + 1; r < m_rows; r++) {
            for(int c = 0; c < m_columns; c++) {
                screen[r][c].character = ' ';
            }
        }
    }
    // mode 1: Від початку екрана до курсора
    else if (mode == 1) {
        for(int r = 0; r < m_cursorRow; r++) {
            for(int c = 0; c < m_columns; c++) {
                screen[r][c].character = ' ';
            }
        }
        clearLine(1);
    }
}

void TerminalBuffer::insertLines(int value) {
    for(int i = 0; i < value; i++) {
        screen.removeLast();
        QVector<Cell> emptyLine(m_columns);
        screen.insert(m_cursorRow, emptyLine);
    }
}

void TerminalBuffer::deleteLines(int value) {
    for(int i = 0; i < value; i++) {
        screen.removeAt(m_cursorRow);
        QVector<Cell> emptyLine(m_columns);
        screen.append(emptyLine);
    }
}
void TerminalBuffer::setScrollRegion(int top, int bottom)
{
    m_marginTop = qBound(0, top, m_rows - 1);
    m_marginBottom = qBound(0, bottom, m_rows - 1);
}

void TerminalBuffer::reverseIndex()
{
    // Якщо ми на верхній межі - зсуваємо текст вниз (щоб звільнити місце зверху)
    if (m_cursorRow == m_marginTop) {
        screen.removeAt(m_marginBottom);
        QVector<Cell> emptyLine(m_columns);
        screen.insert(m_marginTop, emptyLine);
    } else {
        m_cursorRow--;
        if(m_cursorRow < 0) m_cursorRow = 0;
    }
}

void TerminalBuffer::enableAlternateScreen()
{
    if (m_isAlternateScreen) return;
    m_isAlternateScreen = true;

    // Зберігаємо поточний стан (головний екран та позицію курсора)
    m_primaryScreen = screen;
    m_savedCursorRow = m_cursorRow;
    m_savedCursorColumn = m_cursorColumn;

    // Очищаємо екран для запуску програми (наприклад, nano)
    for (int r = 0; r < m_rows; r++) {
        for (int c = 0; c < m_columns; c++) {
            screen[r][c].character = ' ';
        }
    }
    m_cursorRow = 0;
    m_cursorColumn = 0;
    
    // Кажемо таймеру, що треба перемалювати вікно
    dirty = true;
}

void TerminalBuffer::disableAlternateScreen()
{
    if (!m_isAlternateScreen) return;
    m_isAlternateScreen = false;

    // Відновлюємо головний екран та позицію курсора
    screen = m_primaryScreen;
    m_cursorRow = m_savedCursorRow;
    m_cursorColumn = m_savedCursorColumn;

    // Звільняємо пам'ять
    m_primaryScreen.clear();
    
    // Кажемо таймеру, що треба перемалювати вікно
    dirty = true;
}

int TerminalBuffer::lineLength(int row) const
{
    if (row < 0 || row >= totalRows()) {
        return 0;
    }

    int historySize = scrollback.size();
    const QVector<Cell>* targetLine = nullptr;

    if (row < historySize) {
        targetLine = &scrollback[row];
    } else {
        int screenRow = row - historySize;
        if (screenRow >= 0 && screenRow < screen.size()) {
            targetLine = &screen[screenRow];
        }
    }

    if (!targetLine) {
        return 0;
    }

    // Йдемо з кінця рядка на початок і шукаємо перший непустий символ
    for (int col = m_columns - 1; col >= 0; --col) {
        if ((*targetLine)[col].character != ' ') {
            return col + 1; // Повертаємо реальну довжину
        }
    }

    return 0; // Якщо весь рядок пустий
}