import QtQuick
import QtQuick.Window
import "ui"
import "ui/properties"

Window {
    id: root
    width: 900
    height: 600
    visible: true
    title: "EternalTerminal"
    flags: Qt.FramelessWindowHint
    color: "transparent"

    Theme {
        id: themeManager
    }

    property var theme: themeManager.current
    // -------------------------
    Rectangle {
        anchors.fill: parent
        // Головний фон тепер теж прозорий, кольори малюють самі блоки
        color: "transparent"

        Column {
            anchors.fill: parent

            TitleBar {
                width: parent.width
                window: root
                themeManager: themeManager
                color: root.theme.titleBar
                textColor: root.theme.text
                z: 10
            }


            Rectangle {
                width: parent.width
                height: parent.height - 41
                // Читаємо колір фону з нашої теми!
                color: root.theme.background
                // 2. Заокруглюємо нижні кути (якщо вікно розгорнуте - радіус 0)
                radius: root.visibility === Window.Maximized ? 0 : 10
                // 3. Трюк: "заклеюємо" верхні кути цього блоку, щоб вони залишалися гострими
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: parent.radius
                    color: parent.color
                }

                TerminalArea {
                    width: parent.width
                    height: parent.height
                }
                // --- 2. НОВЕ: ВНУТРІШНЯ ТІНЬ ДЛЯ ТЕРМІНАЛА ---
                // Малюється поверх TerminalArea, створює ефект об'єму
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 8 // Товщина нашої тіні
                    
                    // Робимо плавний перехід від напівпрозорого чорного до повністю прозорого
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#66000000" } // Темний край зверху
                        GradientStop { position: 1.0; color: "transparent" } // Розчиняється вниз
                    }
                }
            }

        }

    }
    // Обводка всього вікна (Overlay) ---
        // Лежить поверх усього інтерфейсу, але сам повністю прозорий
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.width: 1
            border.color: root.theme.border
            // Радіус збігається з радіусом нашого вікна
            radius: root.visibility === Window.Maximized ? 0 : 10
            // z: 99 гарантує, що рамка малюється найвище
            z: 99 
        }

    // --- ЗОНИ ЗМІНИ РОЗМІРУ ВІКНА ---
    WindowResizer {
        window: root
    }
}
