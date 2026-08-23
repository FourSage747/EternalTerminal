import QtQuick

QtObject {
    id: themeRoot

    // 1. Стан нашої теми. Змінюючи цей рядок, ми миттєво перемикаємо весь дизайн!
    property string activeTheme: "light"

    // 2. Динамічне посилання. Інші файли будуть читати кольори звідси.
    // Якщо activeTheme == "dark", воно віддасть об'єкт darkTheme, інакше - lightTheme.
    property var current: activeTheme === "dark" ? darkTheme : lightTheme

    // --- ТЕМА 1: ТЕМНА (як класична консоль) ---
    property QtObject darkTheme: QtObject {
        property color background: "#242424"
        property color titleBar: "#171616"
        property color text: "#f0f0f0"
        property color selection: "#5c5c5c"
        property color border: "#3a3a3a"
    }

    // --- ТЕМА 2: СВІТЛА (на майбутнє) ---
    property QtObject lightTheme: QtObject {
        property color background: "#ffffff"
        property color titleBar: "#e0e0e0"
        property color text: "#333333"
        property color selection: "#b3d4fc" 
        property color border: "#cccccc"
    }
    
    // В майбутньому ти просто додаватимеш сюди нові QtObject (наприклад, draculaTheme, monokaiTheme) 
    // і додаватимеш їх у перевірку property var current.
}