#pragma once

#include <QObject>
#include <QString>
#include <QVariantList>

// Попереднє оголошення буфера, щоб ми могли приймати вказівник на нього
class TerminalBuffer; 

class TerminalSelection : public QObject
{
    Q_OBJECT

public:
    explicit TerminalSelection(QObject *parent = nullptr);

    Q_INVOKABLE void begin(int row, int column);
    Q_INVOKABLE void update(int row, int column);
    Q_INVOKABLE void end();
    Q_INVOKABLE void clear();
    Q_INVOKABLE void select(int startRow, int startColumn, int endRow, int endColumn);

    // Нові методи, які ми перенесли з TerminalBuffer
    Q_INVOKABLE QString selectedText(const TerminalBuffer* buffer) const;
    Q_INVOKABLE QVariantList wordAt(const TerminalBuffer* buffer, int row, int column) const;

    bool active() const;
    int startRow() const;
    int startColumn() const;
    int endRow() const;
    int endColumn() const;

    bool contains(int row, int column) const;

private:
    bool m_active = false;
    bool m_dragging = false;

    int m_startRow = 0;
    int m_startColumn = 0;

    int m_endRow = 0;
    int m_endColumn = 0;
};