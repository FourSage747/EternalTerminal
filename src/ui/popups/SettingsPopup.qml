import QtQuick
import QtQuick.Controls

Popup {
        id: themeMenu
        property var themeManager
        width: themeContents.implicitWidth + 16
        height: themeContents.implicitHeight + 16
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        //x: -20
        //y: 40

        background: Rectangle {
            color: themeManager && themeManager.current ? themeManager.current.titleBar : "#1e1e1e"
            border.color: themeManager && themeManager.current ? themeManager.current.border : "#3a3a3a"
            border.width: 1
            radius: 6
        }

        contentItem: Row {
            id: themeContents
            anchors.centerIn: parent
            spacing: 8

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Theme:"
                color: themeManager && themeManager.current ? themeManager.current.text : "#f0f0f0"
                font.pixelSize: 16
            }

            // Кнопка вибору Dark Theme
            Rectangle {
                width: 30
                height: 30
                color: "#333333"
                radius: 50
                border.width: 1
                border.color: root.theme.border

                MouseArea {
                    id: darkMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        if (themeManager) themeManager.activeTheme = "dark"
                        themeMenu.close()
                    }
                }
            }

            // Кнопка вибору Light Theme
            Rectangle {
                width: 30
                height: 30
                color: "#ffffff"
                radius: 50
                border.width: 1
                border.color: root.theme.border

                MouseArea {
                    id: lightMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        if (themeManager) themeManager.activeTheme = "light"
                        themeMenu.close()
                    }
                }
            }
        }
    }