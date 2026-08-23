import QtQuick
import QtQuick.Window

Item {
    id: resizer
    anchors.fill: parent
    z: 100

    property Window window
    
    // Вимикаємо зони, якщо вікно розгорнуте
    enabled: window && window.visibility !== Window.Maximized

    // Товщина зони захоплення
    readonly property int margin: 6

    // 1. Лівий край
    MouseArea {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.margin
        cursorShape: Qt.SizeHorCursor
        onPressed: window.startSystemResize(Qt.LeftEdge)
    }

    // 2. Правий край
    MouseArea {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.margin
        cursorShape: Qt.SizeHorCursor
        onPressed: window.startSystemResize(Qt.RightEdge)
    }

    // 3. Верхній край
    MouseArea {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.margin
        cursorShape: Qt.SizeVerCursor
        onPressed: window.startSystemResize(Qt.TopEdge)
    }

    // 4. Нижній край
    MouseArea {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.margin
        cursorShape: Qt.SizeVerCursor
        onPressed: window.startSystemResize(Qt.BottomEdge)
    }

    // 5. Верхній-лівий кут
    MouseArea {
        anchors.left: parent.left
        anchors.top: parent.top
        width: parent.margin * 2
        height: parent.margin * 2
        cursorShape: Qt.SizeFDiagCursor
        onPressed: window.startSystemResize(Qt.TopEdge | Qt.LeftEdge)
    }

    // 6. Верхній-правий кут
    MouseArea {
        anchors.right: parent.right
        anchors.top: parent.top
        width: parent.margin * 2
        height: parent.margin * 2
        cursorShape: Qt.BDiagCursor
        onPressed: window.startSystemResize(Qt.TopEdge | Qt.RightEdge)
    }

    // 7. Нижній-лівий кут
    MouseArea {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.margin * 2
        height: parent.margin * 2
        cursorShape: Qt.BDiagCursor
        onPressed: window.startSystemResize(Qt.BottomEdge | Qt.LeftEdge)
    }

    // 8. Нижній-правий кут
    MouseArea {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: parent.margin * 2
        height: parent.margin * 2
        cursorShape: Qt.SizeFDiagCursor
        onPressed: window.startSystemResize(Qt.BottomEdge | Qt.RightEdge)
    }
}