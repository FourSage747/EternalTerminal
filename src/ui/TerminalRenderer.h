#pragma once
#include <QPainter>
#include <QQuickPaintedItem>
#include <QTimer>
#include <QString>
#include "../terminal/TerminalSelection.h"

class TerminalBuffer;

class TerminalRenderer : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(
        TerminalBuffer* buffer
        READ buffer
        WRITE setBuffer
        NOTIFY bufferChanged
    )

    Q_PROPERTY(
        TerminalSelection* selection
        READ selection
        WRITE setSelection
        NOTIFY selectionChanged
    )

public:
    explicit TerminalRenderer(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

    TerminalSelection* selection() const;

    void setSelection(TerminalSelection *selection);

    Q_INVOKABLE QPoint cellAt(qreal x, qreal y) const;
    
    // Нові методи для роботи з буфером обміну
    Q_INVOKABLE void copySelection();
    Q_INVOKABLE QString getClipboardText() const;

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    TerminalBuffer* buffer() const;
    void setBuffer(TerminalBuffer *buffer);

signals:
    void bufferChanged();
    void selectionChanged();

private:
    QTimer *cursorTimer = nullptr;
    bool cursorVisible = true;

    TerminalBuffer *m_buffer = nullptr;
    TerminalSelection *m_selection = nullptr;

    // Зберігаємо шрифт як член класу, щоб не створювати його постійно
    QFont m_font;

    int m_cellWidth = 0;
    int m_cellHeight = 0;
    int m_baseline = 0;
    
    void updateFontMetrics();
};