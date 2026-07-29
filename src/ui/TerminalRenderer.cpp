#include "TerminalRenderer.h"

#include "../terminal/TerminalBuffer.h"
#include "../terminal/TerminalSelection.h"

#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QDebug>


TerminalRenderer::TerminalRenderer(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{

    setAntialiasing(false);


    cursorTimer = new QTimer(this);

    cursorTimer->setInterval(500);


    connect(
        cursorTimer,
        &QTimer::timeout,
        this,
        [this]()
        {
            cursorVisible = !cursorVisible;
            update();
        }
    );


    cursorTimer->start();

}


TerminalBuffer* TerminalRenderer::buffer() const
{
    return m_buffer;
}

TerminalSelection* TerminalRenderer::selection() const
{
    return m_selection;
}


void TerminalRenderer::setBuffer(TerminalBuffer *buffer)
{

    if(m_buffer == buffer)
    {
        return;
    }


    m_buffer = buffer;

    connect(
        m_buffer,
        &TerminalBuffer::screenChanged,
        this,
        [this]()
        {
            qDebug() << "RENDER UPDATE";
            update();
        }
    );


    emit bufferChanged();


    update();

}

void TerminalRenderer::setSelection(
    TerminalSelection *selection
)
{

    if(m_selection == selection)
    {
        return;
    }


    m_selection = selection;


    emit selectionChanged();


    update();

}


void TerminalRenderer::paint(QPainter *painter)
{

    qDebug() << "PAINT CALLED";

    if(!m_buffer)
    {
        return;
    }


    QFont font("monospace");

    font.setStyleHint(QFont::TypeWriter);

    font.setPixelSize(16);


    painter->setFont(font);


    painter->setPen(Qt::white);


    QFontMetrics metrics(font);


    const int cellWidth = metrics.horizontalAdvance('M');

    const int cellHeight = metrics.height();

    const int baseline = metrics.ascent();


    // for(int row = 0; row < m_buffer->rows(); row++)
    // {

    //     for(int column = 0; column < m_buffer->columns(); column++)
    //     {

    //         painter->drawText(

    //             column * cellWidth,
    //             row * cellHeight + baseline,

    //             m_buffer->characterAt(row, column)

    //         );

    //     }

    // }
    int historySize = m_buffer->scrollbackSize();
    int offset =
        historySize
        - m_buffer->scrollOffset();


    for(int row = 0; row < m_buffer->rows(); row++)
    {

        int bufferRow = offset + row;


        for(int column = 0; column < m_buffer->columns(); column++)
        {

            ///////
            if(
                m_selection &&
                m_selection->contains(bufferRow, column)
            )
            {
                painter->fillRect(
                    QRect(
                        column * cellWidth,
                        row * cellHeight,
                        cellWidth,
                        cellHeight
                    ),
                    QColor("#44475a")
                );
            }

            ///////

            painter->drawText(

                column * cellWidth,
                row * cellHeight + baseline,
                m_buffer->characterAt(bufferRow, column)

            );

        }

    }


    if(
        cursorVisible &&
        m_buffer->scrollOffset() == 0
    )
    {
        painter->fillRect(

            QRect(

                m_buffer->cursorX() * cellWidth,
                m_buffer->screenCursorY() * cellHeight,

                2,
                cellHeight

            ),

            Qt::white

        );
    }

}

void TerminalRenderer::geometryChange(
    const QRectF &newGeometry,
    const QRectF &oldGeometry
)
{

    QQuickPaintedItem::geometryChange(
        newGeometry,
        oldGeometry
    );


    if(!m_buffer)
        return;


    QFont font("monospace");

    font.setStyleHint(QFont::TypeWriter);

    font.setPixelSize(16);


    QFontMetrics metrics(font);


    m_cellWidth = metrics.horizontalAdvance('M');
    m_cellHeight = metrics.height();
    m_baseline = metrics.ascent();


    int cellWidth =
        metrics.horizontalAdvance('M');


    int cellHeight =
        metrics.height();



    int columns =
        width() / m_cellWidth;


    int rows =
        height() / m_cellHeight;



    if(columns < 1)
        columns = 1;


    if(rows < 1)
        rows = 1;



    qDebug()
        << "TERMINAL RESIZE:"
        << rows
        << "x"
        << columns;



    if(rows > 5 && columns > 5)
    {
        m_buffer->resizeTerminal(
            rows,
            columns
        );
    }

}

QPoint TerminalRenderer::cellAt(
    qreal x,
    qreal y
) const
{

    if(
        !m_buffer ||
        m_cellWidth <= 0 ||
        m_cellHeight <= 0
    )
    {
        return QPoint(0,0);
    }


    int column = int(x) / m_cellWidth;

    int visibleRow = int(y) / m_cellHeight;


    int offset =
        m_buffer->scrollbackSize()
        - m_buffer->scrollOffset();


    int bufferRow =
        offset + visibleRow;


    return QPoint(
        column,
        bufferRow
    );
}