#include "TerminalSelection.h"
#include "TerminalBuffer.h"
#include <utility>


TerminalSelection::TerminalSelection(QObject *parent)
    : QObject(parent)
{

}


void TerminalSelection::begin(
    int row,
    int column
)
{
    m_active = false;

    m_dragging = true;


    m_startRow = row;
    m_startColumn = column;


    m_endRow = row;
    m_endColumn = column;

    // Змушуємо UI миттєво реагувати на рух миші
    emit selectionUpdated();
}



void TerminalSelection::update(
    int row,
    int column,
    qreal exactX,
    qreal exactY  
)
{
    if(!m_dragging)
        return;


    m_endRow = row;
    m_endColumn = column;

    // Зберігаємо точну позицію
    m_exactEndX = exactX;
    m_exactEndY = exactY;

    if(
        row != m_startRow ||
        column != m_startColumn
    )
    {
        m_active = true;
    }

    // Змушуємо UI миттєво реагувати на рух миші
    emit selectionUpdated();
}



void TerminalSelection::clear()
{
    m_active = false;
    m_dragging = false;
    // Змушуємо UI миттєво реагувати на рух миші
    emit selectionUpdated();
}



bool TerminalSelection::active() const
{
    return m_active;
}

void TerminalSelection::end()
{
    m_dragging = false;
}



bool TerminalSelection::contains(
    int row,
    int column
) const
{

    if(!m_active)
        return false;


    int startRow = m_startRow;
    int startColumn = m_startColumn;


    int endRow = m_endRow;
    int endColumn = m_endColumn;


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


    if(row < startRow || row > endRow)
        return false;


    if(startRow == endRow)
    {
        return column >= startColumn &&
               column <= endColumn;
    }


    if(row == startRow)
        return column >= startColumn;


    if(row == endRow)
        return column <= endColumn;


    return true;
}

void TerminalSelection::select(
    int startRow,
    int startColumn,
    int endRow,
    int endColumn
)
{
    m_startRow = startRow;
    m_startColumn = startColumn;

    m_endRow = endRow;
    m_endColumn = endColumn;

    m_active = true;

    m_dragging = false;
    // Змушуємо UI миттєво реагувати на рух миші
    emit selectionUpdated();
}
int TerminalSelection::startRow() const
{
    return m_startRow;
}


int TerminalSelection::startColumn() const
{
    return m_startColumn;
}


int TerminalSelection::endRow() const
{
    return m_endRow;
}


int TerminalSelection::endColumn() const
{
    return m_endColumn;
}

QString TerminalSelection::selectedText(const TerminalBuffer* buffer) const
{
    if(!buffer || !m_active)
        return "";

    int startR = m_startRow;
    int startC = m_startColumn;
    int endR = m_endRow;
    int endC = m_endColumn;

    if(startR > endR || (startR == endR && startC > endC))
    {
        std::swap(startR, endR);
        std::swap(startC, endC);
    }

    QString result;
    int cols = buffer->columns(); // Беремо кількість колонок з буфера

    for(int row = startR; row <= endR; row++)
    {
        // Беремо реальну довжину тексту в рядку
        int lineLen = buffer->lineLength(row);
        
        int from = 0;
        // Обмежуємо копіювання кінцем реального тексту
        int to = lineLen - 1; 

        if(row == startR) from = startC;
        if(row == endR) to = std::min(to, endC);

        for(int col = from; col <= to; col++)
        {
            // Використовуємо універсальний метод буфера
            result += buffer->characterAt(row, col);
        }

        if(row != endR)
            result += "\n";
    }

    return result.trimmed();
}

QVariantList TerminalSelection::wordAt(const TerminalBuffer* buffer, int row, int column) const
{
    QVariantList result;

    if(!buffer || row < 0 || row >= buffer->totalRows() || column < 0 || column >= buffer->columns())
    {
        return result;
    }

    QString line;
    int cols = buffer->columns();

    for(int x = 0; x < cols; x++)
    {
        line += buffer->characterAt(row, x);
    }

    if(line[column].isSpace())
    {
        return result;
    }

    int start = column;
    while(start > 0 && !line[start - 1].isSpace())
    {
        start--;
    }

    int end = column;
    while(end < cols - 1 && !line[end + 1].isSpace())
    {
        end++;
    }

    result.append(row);
    result.append(start);
    result.append(row);
    result.append(end);

    return result;
}

bool TerminalSelection::isDragging() const
{
    return m_dragging;
}

qreal TerminalSelection::exactEndX() const
{
    return m_exactEndX;
}

qreal TerminalSelection::exactEndY() const
{
    return m_exactEndY;
}