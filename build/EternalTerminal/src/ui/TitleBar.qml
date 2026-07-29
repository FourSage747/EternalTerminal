import QtQuick


Rectangle {

    id: bar

    height: 40

    color: "#1a1a1a"


    property var window


    Text {

        anchors.centerIn: parent

        text: "EternalTerminal"

        color: "#eeeeee"

        font.pixelSize: 16

    }

    Rectangle {

        anchors.left: parent.left
        anchors.right: buttons.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom


        color: "transparent"


        MouseArea {

            anchors.fill: parent


            property point clickPos


            onPressed:
            {
                clickPos = Qt.point(mouse.x, mouse.y)
            }


            onPositionChanged:
            {
                if(mouse.buttons & Qt.LeftButton)
                {
                    window.x += mouse.x - clickPos.x
                    window.y += mouse.y - clickPos.y
                }
            }

        }

    }



    Row {

        id: buttons

        anchors.right: parent.right

        anchors.verticalCenter: parent.verticalCenter


        spacing: 4


        WindowButton {

            icon: "minimize"

            onClicked:
            {
                window.showMinimized()
            }

        }


        WindowButton {

            icon: "maximize"

            onClicked:
            {
                if(window.visibility === Window.Maximized)
                    window.showNormal()
                else
                    window.showMaximized()
            }

        }



        WindowButton {

            icon: "close"

            danger: true

            onClicked:
            {
                window.close()
            }

        }


    }


}