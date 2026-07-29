import QtQuick
import EternalTerminal 1.0

Rectangle {

    color: "#0d0d0d"

    property var engine: terminalEngine

    focus: true

    Timer {

        id: autoScrollTimer

        interval: 120

        repeat: true

        running: false

        onTriggered: {

            if(!renderer.buffer)
                return;


            if(!mouseArea.selecting)
                return;

            const lastMouseY = mouseArea.lastMouseY;
            const lastMouseX = mouseArea.lastMouseX;

            if(lastMouseY < 10)
            {
                let speed = Math.ceil((30 - lastMouseY) / 10);

                terminalEngine.terminalBuffer.scroll(
                    Math.min(speed, 3)
                );

                let cell = renderer.cellAt(
                    lastMouseX,
                    0
                );

                terminalEngine.selection.update(
                    terminalEngine.terminalBuffer.bufferRowFromScreenRow(
                        cell.y
                    ),
                    cell.x
                );
            }


            else if(lastMouseY > height - 10)
            {
                let speed = Math.ceil((lastMouseY - (height - 30)) / 10);

                terminalEngine.terminalBuffer.scroll(
                    Math.min(-speed, -3)
                );

                let cell = renderer.cellAt(
                    lastMouseX,
                    height - 1
                );

                terminalEngine.selection.update(
                    terminalEngine.terminalBuffer.bufferRowFromScreenRow(
                        cell.y
                    ),
                    cell.x
                );
            }

        }

    }



    TerminalRenderer {

        id: renderer

        anchors.fill: parent

        buffer: engine ? engine.terminalBuffer : null

        selection: engine ? engine.selection : null
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

            if(wheel.angleDelta.y > 0)
            {
                terminalEngine.terminalBuffer.scroll(5);
            }
            else if(wheel.angleDelta.y < 0)
            {
                terminalEngine.terminalBuffer.scroll(-5);
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

            //======Work=====
            terminalEngine.selection.update(
                cell.y,
                cell.x
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


            let word = terminalEngine.terminalBuffer.wordAt(
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

            terminalEngine.copySelection();

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
            terminalEngine.paste();

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