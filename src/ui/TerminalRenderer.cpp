#include "TerminalRenderer.h"

#include "../terminal/TerminalBuffer.h"
#include "../terminal/TerminalSelection.h"

#include <QFontDatabase>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QDebug>
#include <QGuiApplication>
#include <QClipboard>


TerminalRenderer::TerminalRenderer(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{

    setAntialiasing(false);

    // Ініціалізація шрифту один раз
    m_font = QFont("monospace");
    m_font.setStyleHint(QFont::TypeWriter);
    m_font.setKerning(false);
    m_font.setPixelSize(16);
    updateFontMetrics();

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

void TerminalRenderer::updateFontMetrics()
{
    // 1. Просимо ОС дати ідеальний фіксований шрифт
    m_font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    m_font.setPixelSize(16);
    m_font.setFixedPitch(true);
    m_font.setKerning(false);

    // 2. Рахуємо точні розміри в qreal (без втрати мікро-пікселів)
    QFontMetricsF metrics(m_font);
    m_cellWidth = metrics.horizontalAdvance('M');
    m_cellHeight = metrics.height();
    m_baseline = metrics.ascent();
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

qreal TerminalRenderer::visualScrollOffset() const
{
    return m_visualScrollOffset;
}

void TerminalRenderer::setVisualScrollOffset(qreal offset)
{
    if(qFuzzyCompare(m_visualScrollOffset, offset))
        return;

    m_visualScrollOffset = offset;
    emit visualScrollOffsetChanged();
    
    // Обов'язково викликаємо update(), щоб перемалювати екран з новим зсувом
    update();
}   

void TerminalRenderer::setSelection(TerminalSelection *selection)
{
    if(m_selection == selection) return;

    // Відключаємо старі сигнали, щоб не було витоку пам'яті
    if (m_selection) {
        m_selection->disconnect(this);
    }

    m_selection = selection;

    // НОВЕ: Підключаємо сигнал оновлення до миттєвого перемальовування екрана
    if (m_selection) {
        connect(m_selection, &TerminalSelection::selectionUpdated, this, [this]() {
            update();
        });
    }

    emit selectionChanged();
    update();
}

void TerminalRenderer::paint(QPainter *painter)
{
    if(!m_buffer || m_cellWidth <= 0 || m_cellHeight <= 0) return;

    painter->setFont(m_font);
    painter->setPen(m_textColor);

    int historySize = m_buffer->scrollbackSize();
    int integerOffset = static_cast<int>(m_visualScrollOffset);
    qreal fractionalOffset = m_visualScrollOffset - integerOffset;

    int offset = historySize - integerOffset;
    qreal pixelShift = fractionalOffset * m_cellHeight; 
    
    int cols = m_buffer->columns();
    
    // Підганяємо розмір буфера без перестворення пам'яті
    if (m_lineBuffer.size() != cols) {
        m_lineBuffer.resize(cols);
    }

    for(int row = -1; row <= m_buffer->rows(); row++)
    {
        int bufferRow = offset + row;
        if (bufferRow < 0 || bufferRow >= m_buffer->totalRows()) continue;

        qreal yPos = row * m_cellHeight + pixelShift; 

        // Отримуємо реальну довжину рядка без зайвих пробілів
        int lineLen = m_buffer->lineLength(bufferRow);

        for(int column = 0; column < cols; column++)
        {
            if(m_selection && m_selection->contains(bufferRow, column))
            {
                // Малюємо фон ТІЛЬКИ якщо це реальний текст (або символ кінця рядка)
                if (lineLen > 0 && column <= lineLen)
                {
                    qreal rectX = column * m_cellWidth;
                    qreal rectWidth = m_cellWidth;

                    // Плавне виділення: якщо ми зараз тягнемо мишку і це кінцева клітинка
                    if (m_selection->isDragging() && 
                        bufferRow == m_selection->endRow() && 
                        column == m_selection->endColumn())
                    {
                        // Визначаємо напрямок виділення (зліва-направо чи справа-наліво)
                        bool isForward = (m_selection->startRow() < m_selection->endRow()) || 
                                        (m_selection->startRow() == m_selection->endRow() && m_selection->startColumn() <= m_selection->endColumn());

                        if (isForward) {
                            // Малюємо фон від початку клітинки рівно до позиції миші
                            rectWidth = qBound(0.0, m_selection->exactEndX() - rectX, m_cellWidth);
                        } else {
                            // Якщо виділяємо у зворотний бік - малюємо від миші до кінця клітинки
                            qreal exactX = qBound(rectX, m_selection->exactEndX(), rectX + m_cellWidth);
                            rectWidth = (rectX + m_cellWidth) - exactX;
                            rectX = exactX;
                        }
                    }

                    painter->fillRect(
                        QRectF(rectX, yPos, rectWidth, m_cellHeight),
                        m_selectionColor
                    );
                }
            }

            // Блискавично перезаписуємо символ у вже існуючій пам'яті
            m_lineBuffer[column] = m_buffer->rawCharAt(bufferRow, column);
        }

        // Малюємо всю лінію
        painter->drawText(QPointF(0, yPos + m_baseline), m_lineBuffer);
    }

    if(cursorVisible && integerOffset == 0)
    {
        painter->fillRect(
            QRectF(m_buffer->cursorX() * m_cellWidth, (m_buffer->screenCursorY() * m_cellHeight) + pixelShift, 2.0, m_cellHeight),
            m_textColor
        );
    }
}

void TerminalRenderer::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickPaintedItem::geometryChange(newGeometry, oldGeometry);

    if(!m_buffer) return;

    updateFontMetrics();

    if(m_cellWidth <= 0 || m_cellHeight <= 0) return;

    int columns = width() / m_cellWidth;
    int rows = height() / m_cellHeight;

    if(columns < 1) columns = 1;
    if(rows < 1) rows = 1;

    qDebug() << "TERMINAL RESIZE:" << rows << "x" << columns;

    if(rows > 5 && columns > 5)
    {
        m_buffer->resizeTerminal(rows, columns);
    }
}

QPoint TerminalRenderer::cellAt(qreal x, qreal y) const
{
    if(!m_buffer || m_cellWidth <= 0 || m_cellHeight <= 0) return QPoint(0,0);

    qreal fractionalOffset = m_visualScrollOffset - static_cast<int>(m_visualScrollOffset);
    int pixelShift = fractionalOffset * m_cellHeight;

    int column = int(x) / m_cellWidth;
    
    // Оскільки текст візуально зсунувся вниз (+pixelShift), ми маємо відняти це від координати миші
    int visibleRow = 0;
    if (y - pixelShift >= 0) {
        visibleRow = int(y - pixelShift) / m_cellHeight;
    } else {
        visibleRow = -1; // Для безпеки, якщо курсор миші вище першого рядка
    }

    int integerOffset = static_cast<int>(m_visualScrollOffset);
    int offset = m_buffer->scrollbackSize() - integerOffset;
    int bufferRow = offset + visibleRow;

    return QPoint(column, bufferRow);
}

void TerminalRenderer::copySelection()
{
    if(!m_selection || !m_buffer) return;

    QString text = m_selection->selectedText(m_buffer);
    if(text.isEmpty()) return;

    QGuiApplication::clipboard()->setText(text);
}

QString TerminalRenderer::getClipboardText() const
{
    return QGuiApplication::clipboard()->text();
}

QColor TerminalRenderer::textColor() const { return m_textColor; }
void TerminalRenderer::setTextColor(const QColor &color) {
    if (m_textColor == color) return;
    m_textColor = color;
    emit textColorChanged();
    update();
}

QColor TerminalRenderer::selectionColor() const { return m_selectionColor; }
void TerminalRenderer::setSelectionColor(const QColor &color) {
    if (m_selectionColor == color) return;
    m_selectionColor = color;
    emit selectionColorChanged();
    update();
}