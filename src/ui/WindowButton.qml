import QtQuick


Rectangle {

    id: root


    width: 42

    height: 40


    property string icon: ""

    property bool danger: false



    signal clicked()



    color:
        mouse.containsMouse
        ?
        (danger ? "#c42b1c" : "#333333")
        :
        "transparent"



    Canvas {

        anchors.centerIn: parent

        width: 14

        height: 14



        onPaint:
        {

            let ctx = getContext("2d")


            ctx.clearRect(
                0,
                0,
                width,
                height
            )


            ctx.strokeStyle = "#dddddd"

            ctx.lineWidth = 1.5

            ctx.lineCap = "square"



            if(root.icon === "minimize")
            {

                ctx.beginPath()

                ctx.moveTo(1,7)

                ctx.lineTo(13,7)

                ctx.stroke()

            }


            else if(root.icon === "maximize")
            {

                ctx.strokeRect(
                    2,
                    2,
                    10,
                    10
                )

            }


            else if(root.icon === "close")
            {

                ctx.beginPath()

                ctx.moveTo(2,2)

                ctx.lineTo(12,12)

                ctx.moveTo(12,2)

                ctx.lineTo(2,12)

                ctx.stroke()

            }

        }

    }



    MouseArea {

        id: mouse

        anchors.fill: parent

        hoverEnabled: true


        onClicked:
        {
            root.clicked()
        }

    }


}
