import QtQuick
import QtQuick.Controls
import "../popups"
Item {
    id: controls
    width: buttonRow.width
    height: 40

    property var window
    property var themeManager

    Row {
        id: buttonRow
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        spacing: 8
        anchors.rightMargin: 10

        // Кнопка відкриття меню тем
        WindowButton {
            icon: "menu"
            themeManager: controls.themeManager
            onClicked: {
                // Відкриваємо або закриваємо Popup
                if (settingsPopup.opened)
                    settingsPopup.close()
                else
                    settingsPopup.open()
            }
        }

        WindowButton {
            icon: "minimize"
            themeManager: controls.themeManager
            onClicked: {
                if (window) window.showMinimized()
            }
        }

        WindowButton {
            icon: "maximize"
            themeManager: controls.themeManager
            onClicked: {
                if (window) {
                    if (window.visibility === Window.Maximized)
                        window.showNormal()
                    else
                        window.showMaximized()
                }
            }
        }

        WindowButton {
            icon: "close"
            danger: true
            themeManager: controls.themeManager
            onClicked: {
                if (window) window.close()
            }
        }
    }

    // Випадаюче модальне віконце вибору теми
    SettingsPopup {
        id: settingsPopup
        x: -20
        y: 40
        themeManager: controls.themeManager 
    }
}