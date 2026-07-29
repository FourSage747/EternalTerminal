#include "TerminalSelection.h"


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
}



void TerminalSelection::update(
    int row,
    int column
)
{
    if(!m_dragging)
        return;


    m_endRow = row;
    m_endColumn = column;

    if(
        row != m_startRow ||
        column != m_startColumn
    )
    {
        m_active = true;
    }
}



void TerminalSelection::clear()
{
    m_active = false;
    m_dragging = false;
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