import QtQuick
import EternalTerminal 1.0

Rectangle {
    color: "transparent"
    property var engine: terminalEngine
    focus: true
    property real targetScroll: 0

    Connections {
        target: engine ? engine.terminalBuffer : null
        
        // Оновлюємо, коли користувач скролить мишкою
        function onScrollChanged() {
            targetScroll = engine.terminalBuffer.scrollOffset()
        }
        
        // Оновлюємо, коли буфер сам змінює розмір (наприклад, при виводі нового тексту)
        function onScreenChanged() {
            targetScroll = engine.terminalBuffer.scrollOffset()
        }
    }

    Timer {

        id: autoScrollTimer

        interval: 16 // ~60 FPS для дуже плавної анімації

        repeat: true

        running: false

        onTriggered: {

            if(!renderer.buffer)
                return;


            if(!mouseArea.selecting)
                return;

            const lastMouseY = mouseArea.lastMouseY;
            const lastMouseX = mouseArea.lastMouseX;
            let scrollAmount = 0.0;

            // Зона автоскролу: 20 пікселів від країв вікна
            if(lastMouseY < 20) {
                // Чим далі мишка, тим швидше дробовий скрол (до 0.6 рядка за кадр)
                scrollAmount = (20 - lastMouseY) * 0.03;
            } else if(lastMouseY > height - 20) {
                scrollAmount = -((lastMouseY - (height - 20)) * 0.03);
            }

            if(scrollAmount !== 0.0) {
                // Плавно скролимо на дробову кількість рядків
                terminalEngine.terminalBuffer.scroll(scrollAmount);

                // Залишаємо координату Y в межах екрана, щоб виділення не "втікало"
                let clampedY = Math.max(0, Math.min(lastMouseY, height - 1));
                let cell = renderer.cellAt(lastMouseX, clampedY);

                // Оновлюємо виділення, передаючи точні координати миші
                terminalEngine.selection.update(
                    cell.y,
                    cell.x,
                    lastMouseX,
                    lastMouseY
                );
            }

        }

    }



    TerminalRenderer {
        id: renderer
        anchors.fill: parent
        anchors.margins: 8
        buffer: engine ? engine.terminalBuffer : null
        selection: engine ? engine.selection : null
        // Передаємо нашу цільову позицію в C++ властивість
        visualScrollOffset: targetScroll
        // Передаємо кольори з теми у C++
        textColor: root.theme.text
        selectionColor: root.theme.selection
    }

    MouseArea {

        id: mouseArea

        anchors.fill: parent

        acceptedButtons: Qt.LeftButton

        preventStealing: true

        hoverEnabled: true

        property bool selecting: false
        property real lastMouseX: 0
        property real lastMouseY: 0

        onWheel: function(wheel) {
            let lines = 0;
            
            // Якщо це тачпад (апаратний плавний скрол у пікселях)
            if (wheel.pixelDelta.y !== 0) {
                // 18.0 - приблизна висота рядка. Мікро-рух тачпада = мікро-зсув тексту
                lines = wheel.pixelDelta.y / 18.0; 
            } 
            // Якщо це звичайна мишка (крок коліщатка = 120 одиниць)
            else {
                lines = (wheel.angleDelta.y / 120.0) * 3.0; // 3 рядки за один клік
            }

            if (lines !== 0) {
                terminalEngine.terminalBuffer.scroll(lines);
            }

            wheel.accepted = true;
        }

        onPressed: function(mouse)
        {
            lastMouseX = mouse.x
            lastMouseY = mouse.y

            selecting = true;

            terminalEngine.selection.clear();

            let cell = renderer.cellAt(mouse.x, mouse.y)

            //====WOrk====
            terminalEngine.selection.begin(
                cell.y,
                cell.x
            )


            mouse.accepted = true;
            autoScrollTimer.start()
        }

        onPositionChanged: function(mouse)
        {
            lastMouseX = mouse.x
            lastMouseY = mouse.y
            if(!selecting)
                return;

            let cell = renderer.cellAt(mouse.x, mouse.y)

            // Передаємо точні пікселі миші
            terminalEngine.selection.update(
                cell.y,
                cell.x,
                mouse.x,
                mouse.y
            )
        }

        onReleased: function(mouse)
        {
            selecting = false;
            terminalEngine.selection.end();
            autoScrollTimer.stop()
        }

        onDoubleClicked: function(mouse)
        {
            selecting = false;

            let cell = renderer.cellAt(
                mouse.x,
                mouse.y
            )


            // ДОДАЛИ terminalEngine.terminalBuffer першим аргументом!
            let word = terminalEngine.selection.wordAt(
                terminalEngine.terminalBuffer,
                cell.y,
                cell.x
            )


            if(word.length === 4)
            {
                terminalEngine.selection.select(
                    word[0],
                    word[1],
                    word[2],
                    word[3]
                )
            }
        }

    }



    Component.onCompleted: {

        forceActiveFocus()

        terminalEngine.startShell()

    }



    Keys.onPressed: function(event) {
        //COPY
        if(
            event.modifiers & Qt.ControlModifier &&
            event.modifiers & Qt.ShiftModifier &&
            event.key === Qt.Key_C
        )
        {

            renderer.copySelection(); // Викликаємо через renderer
            event.accepted = true;
            return;
        }
        //PASTE
        if(
            event.modifiers & Qt.ControlModifier &&
            event.modifiers & Qt.ShiftModifier &&
            event.key === Qt.Key_V
        )
        {
            // Беремо текст з renderer і відправляємо в engine
            let text = renderer.getClipboardText();
            if(text !== "") {
                terminalEngine.sendInput(text);
            }
            event.accepted = true;
            return;
        }

        ////\\\\\ Manual scroll test\\\\\/////====
        // if(event.key === Qt.Key_Up)
        // {
        //     terminalEngine.terminalBuffer.scroll(5);

        //     event.accepted = true;
        //     return;
        // }


        // if(event.key === Qt.Key_Down)
        // {
        //     terminalEngine.terminalBuffer.scroll(-5);

        //     event.accepted = true;
        //     return;
        // }
        /////\\\\\\\///////\\\\\\\\\=========

        let text = "";

        if(event.key === Qt.Key_Return)
        {
            text = "\r";
        }
        else if(event.key === Qt.Key_Backspace)
        {
            text = "\x7f";
        }
        else if(event.key === Qt.Key_Left)
        {
            text = "\x1b[D";
        }
        else if(event.key === Qt.Key_Right)
        {
            text = "\x1b[C";
        }
        else if(event.key === Qt.Key_Up)
        {
            text = "\x1b[A";
        }
        else if(event.key === Qt.Key_Down)
        {
            text = "\x1b[B";
        }
        else
        {
            text = event.text;
        }


        if(text.length > 0)
        {
            console.log("SEND:", JSON.stringify(text));
            terminalEngine.sendInput(text);
        }


        event.accepted = true;

    }

}