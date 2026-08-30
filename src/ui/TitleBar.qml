import QtQuick
import "controlButtons"

Rectangle {
    id: bar
    height: 40

    property var window
    property var themeManager
    property var version: "0.2"
    property color textColor: "#eeeeee"

    radius: window.visibility === Window.Maximized ? 0 : 10

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.radius
        color: parent.color
    }

    Text {
        anchors.centerIn: parent
        text: "EternalTerminal v" + version
        color: textColor
        font.pixelSize: 16
    }

    // Область перетягування вікна
    Rectangle {
        anchors.left: parent.left
        anchors.right: controls.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: "transparent"

        MouseArea {
            anchors.fill: parent
            onPressed: {
                if (window) window.startSystemMove()
            }
        }
    }

    // --- НОВИЙ ОДНИЙ КОМПОНЕНТ КНОПОК ---
    WindowControls {
        id: controls
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        window: bar.window
        themeManager: bar.themeManager
    }
}