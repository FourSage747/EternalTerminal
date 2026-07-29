import QtQuick
import QtQuick.Window
import "ui"


Window {
    id: root
    width: 900
    height: 600

    visible: true

    title: "EternalTerminal"
    flags: Qt.FramelessWindowHint

    Rectangle {

        anchors.fill: parent

        color: "#111111"


        Column {

            anchors.fill: parent


            TitleBar {

                width: parent.width
                window: root
            }


            Rectangle {

                width: parent.width

                height: parent.height - 40

                color: "#0d0d0d"


                TerminalArea {

                    width: parent.width

                    height: parent.height

                }

            }

        }

    }

}
