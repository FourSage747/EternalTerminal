#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QTimer>
#include <QVariant>

class TerminalParser;

class TerminalBuffer : public QObject
{
    Q_OBJECT


    Q_PROPERTY(
        int cursorX
        READ cursorX
        NOTIFY cursorChanged
    )


    Q_PROPERTY(
        int cursorY
        READ cursorY
        NOTIFY cursorChanged
    )


    Q_PROPERTY(
        int rows
        READ rows
        NOTIFY screenChanged
    )


    Q_PROPERTY(
        int columns
        READ columns
        NOTIFY screenChanged
    )


public:

    explicit TerminalBuffer(QObject *parent = nullptr);


    int cursorX() const;
    int cursorY() const;

    int rows() const;
    int columns() const;

    int screenCursorY() const;


    Q_INVOKABLE
    QString characterAt(int row, int column) const;

    Q_INVOKABLE
    int scrollbackSize() const;


    Q_INVOKABLE
    void append(QString data);


    Q_INVOKABLE
    void resizeTerminal(int rows, int columns);

    Q_INVOKABLE
    int totalRows() const;

    Q_INVOKABLE
    int visibleOffset() const;

    Q_INVOKABLE
    int bufferRowFromScreenRow(
        int screenRow
    ) const;

    Q_INVOKABLE
    void scroll(int amount);

    Q_INVOKABLE
    int scrollOffset() const;

    void processCharacter(QChar c);
    void moveCursorUp(int value);
    void moveCursorDown(int value);
    void moveCursorRight(int value);
    void moveCursorLeft(int value);
    void clearLine(int mode);
    void deleteCharacters(int value);
    void insertBlankCharacters(int value);

signals:

    void screenChanged();

    void cursorChanged();

    void scrollChanged();

    void resized(int rows, int columns);


private:

    TerminalParser* m_parser = nullptr;
    struct Cell
    {
        QChar character = ' ';
    };


    QVector<QVector<Cell>> screen;

    QVector<QVector<Cell>> scrollback;


    int m_rows = 24;
    int m_columns = 80;


    int m_cursorRow = 0;
    int m_cursorColumn = 0;
    int m_lineEndColumn = 0;
    int m_inputStartColumn = 0;
    int m_scrollOffset = 0;
    int m_lastScrollbackSize = 0;
    bool m_followOutput = true;

    QTimer *updateTimer = nullptr;

    bool dirty = false;



    void resize(int rows, int columns);

    void reflowScreen(int oldColumns);


    void putCharacter(QChar c);


};