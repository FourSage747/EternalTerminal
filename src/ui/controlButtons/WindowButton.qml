import QtQuick

Rectangle {
    id: root

    width: 30
    height: 30
    radius: 50

    property string icon: ""
    property bool danger: false
    property var themeManager
    property color strokeColor: themeManager && themeManager.current ? themeManager.current.text : "#eeeeee"
    property color hoverColor: themeManager && themeManager.current ? themeManager.current.border : "#eeeeee"
    signal clicked()

    onStrokeColorChanged: {
        canvas.requestPaint()
    }

    color: mouse.containsMouse ? root.hoverColor : "transparent"

    Canvas {
        id: canvas
        anchors.centerIn: parent
        width: 14
        height: 14

        onPaint: {
            let ctx = getContext("2d")

            ctx.clearRect(0, 0, width, height)

            ctx.strokeStyle = root.strokeColor
            ctx.fillStyle = root.strokeColor
            ctx.lineWidth = 1.5
            ctx.lineCap = "square"

            if (root.icon === "minimize") {
                ctx.beginPath()
                ctx.moveTo(1, 7)
                ctx.lineTo(13, 7)
                ctx.stroke()
            }
            else if (root.icon === "maximize") {
                ctx.strokeRect(2, 2, 10, 10)
            }
            else if (root.icon === "close") {
                ctx.beginPath()
                ctx.moveTo(2, 2)
                ctx.lineTo(12, 12)
                ctx.moveTo(12, 2)
                ctx.lineTo(2, 12)
                ctx.stroke()
            }
            // --- НОВЕ: Малюємо 3 вертикальні крапки ---
            else if (root.icon === "menu" || root.icon === "more") {
                ctx.beginPath()
                ctx.arc(7, 3, 1.5, 0, 2 * Math.PI)
                ctx.arc(7, 7, 1.5, 0, 2 * Math.PI)
                ctx.arc(7, 11, 1.5, 0, 2 * Math.PI)
                ctx.fill()
            }
        }
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            root.clicked()
        }
    }
}