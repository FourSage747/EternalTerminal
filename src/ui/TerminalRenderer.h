#pragma once
#include <QPainter>
#include <QQuickPaintedItem>
#include <QTimer>
#include "../terminal/TerminalSelection.h"

class TerminalBuffer;
// class TerminalSelection;

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

    void setSelection(
        TerminalSelection *selection
    );

    Q_INVOKABLE
    QPoint cellAt(
        qreal x,
        qreal y
    ) const;

protected:

    void geometryChange(
        const QRectF &newGeometry,
        const QRectF &oldGeometry
    ) override;

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

    int m_cellWidth = 0;
    int m_cellHeight = 0;
    int m_baseline = 0;
};
