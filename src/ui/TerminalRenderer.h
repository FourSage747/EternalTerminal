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

    Q_PROPERTY(
        qreal visualScrollOffset
        READ visualScrollOffset
        WRITE setVisualScrollOffset
        NOTIFY visualScrollOffsetChanged
    )

    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor NOTIFY selectionColorChanged)

public:
    explicit TerminalRenderer(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

    TerminalSelection* selection() const;

    void setSelection(TerminalSelection *selection);

    Q_INVOKABLE QPoint cellAt(qreal x, qreal y) const;
    
    // Нові методи для роботи з буфером обміну
    Q_INVOKABLE void copySelection();
    Q_INVOKABLE QString getClipboardText() const;

    qreal visualScrollOffset() const;
    void setVisualScrollOffset(qreal offset);

    QColor textColor() const;
    void setTextColor(const QColor &color);
    QColor selectionColor() const;
    void setSelectionColor(const QColor &color);

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    TerminalBuffer* buffer() const;
    void setBuffer(TerminalBuffer *buffer);

signals:
    void bufferChanged();
    void selectionChanged();
    void visualScrollOffsetChanged();
    void textColorChanged();
    void selectionColorChanged();

private:
    QTimer *cursorTimer = nullptr;
    bool cursorVisible = true;

    TerminalBuffer *m_buffer = nullptr;
    TerminalSelection *m_selection = nullptr;

    // Зберігаємо шрифт як член класу, щоб не створювати його постійно
    QFont m_font;

    qreal m_cellWidth = 0.0;
    qreal m_cellHeight = 0.0;
    qreal m_baseline = 0.0;
    qreal m_visualScrollOffset = 0.0;

    QString m_lineBuffer;
    
    void updateFontMetrics();

    QColor m_textColor = Qt::white;
    QColor m_selectionColor = QColor("#44475a");
};