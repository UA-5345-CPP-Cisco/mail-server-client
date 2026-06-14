import QtQuick
import QtQuick.Shapes
import QtQuick.Controls
Rectangle
{
    id: newMessageQML
    implicitHeight: 398
    implicitWidth: 420
    border.color: "#e5e7eb"

    clip: true
    color: "#fcf3e6"
    radius: 14

    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            newMessageQML.forceActiveFocus()
        }
    }

    Rectangle
    {
        id: background

        height: 38
        width: 420
        topLeftRadius: 14
        topRightRadius: 14

        color: "#1e2939"
        MouseArea
        {
            anchors.fill: parent
            cursorShape: Qt.OpenHandCursor

            drag.target: newMessageQML
            drag.axis: Drag.XAndYAxis

            onPressed: cursorShape = Qt.ClosedHandCursor
            onReleased: cursorShape = Qt.OpenHandCursor
        }

        Rectangle {
            id: drag_container

            x: 12
            y: 9

            height: 20
            width: 306

            clip: true
            color: "transparent"

            Text {
                id: new_Message

                height: 20
                width: 307

                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                text: "New Message"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }
        }

        Rectangle {
            id: windowControlsWrapper


            x: 320
            y: 8
            width: 140
            height: 45
            color: "transparent"

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.ArrowCursor

            }

            Row
            {
                id:new_message_controls
                x: 340
                y: 8
                anchors.fill: parent
                spacing: 8
                //button to minimize window
                Rectangle
                {
                    id: buttonToMinimizeWindow
                    height: 22
                    width: 22

                    color: clickAreaMinimizeWindow.pressed ? "#ffdede" : "transparent"
                    radius: 4

                    MouseArea
                    {
                        id: clickAreaMinimizeWindow
                        anchors.fill: parent

                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked:
                        {
                            // MinimizeWindow
                        }
                    }


            Rectangle {
                id: sVG

                x: 4
                y: 4

                height: 14
                width: 14

                clip: true
                color: "transparent"
                scale: clickAreaMinimizeWindow.containsMouse ? 1.5 : 1.0

                Behavior on scale {
                    NumberAnimation {
                        duration: 150
                        easing.type: Easing.InOutQuad
                    }
                }

                Shape {
                    id: _vector

                    x: 2.92
                    y: 7

                    height: 0
                    width: 8.17

                    ShapePath {
                        id: _vector_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_ShapePath0_PathSvg0

                            path: "M 0 0 L 8.166666030883789 0"
                        }
                    }
                }
            }
        }

                //button to maximize window
                Rectangle
                {
                id: buttonToMaximizeWindow

                height: 22
                width: 22

                color: clickAreaMaximizeWindow.pressed ? "#ffdede" : "transparent"

                MouseArea
                {
                    id: clickAreaMaximizeWindow
                    anchors.fill: parent

                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked:
                    {
                        //MaximizeWindow
                    }
                }

            radius: 4

            Rectangle
            {
                id: sVG_1

                x: 4
                y: 4

                height: 14
                width: 14

                clip: true
                color: "transparent"
                scale: clickAreaMaximizeWindow.containsMouse ? 1.5 : 1.0

                Behavior on scale
                {
                    NumberAnimation
                    {
                        duration: 150
                        easing.type: Easing.InOutQuad
                    }
                }

                Shape {
                    id: _vector_1

                    x: 8.75
                    y: 1.75

                    height: 3.50
                    width: 3.50

                    ShapePath {
                        id: _vector_1_ShapePath0

                        fillColor: "#00000000"
                        fillRule: ShapePath.WindingFill
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_1_ShapePath0_PathSvg0

                            path: "M 0 0 L 3.5 0 L 3.5 3.5"
                        }
                    }
                }
                Shape {
                    id: _vector_2

                    x: 1.75
                    y: 8.75

                    height: 3.50
                    width: 3.50

                    ShapePath {
                        id: _vector_2_ShapePath0

                        fillColor: "#00000000"
                        fillRule: ShapePath.WindingFill
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_2_ShapePath0_PathSvg0

                            path: "M 3.5 3.5 L 0 3.5 L 0 0"
                        }
                    }
                }
                Shape {
                    id: _vector_3

                    x: 8.17
                    y: 1.75

                    height: 4.08
                    width: 4.08

                    ShapePath {
                        id: _vector_3_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_3_ShapePath0_PathSvg0

                            path: "M 4.0833330154418945 0 L 0 4.0833330154418945"
                        }
                    }
                }
                Shape {
                    id: _vector_4

                    x: 1.75
                    y: 8.17

                    height: 4.08
                    width: 4.08

                    ShapePath {
                        id: _vector_4_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_4_ShapePath0_PathSvg0

                            path: "M 0 4.0833330154418945 L 4.0833330154418945 0"
                        }
                    }
                }
            }
        }

                //button to close window
                Rectangle
                {
                id: buttonToCloseWindow
                height: 22
                width: 22
                radius: 4

                color: clickAreaCloseMessageBox.pressed ? "#ffdede" : "transparent"

                MouseArea
                {
                    id: clickAreaCloseMessageBox
                    anchors.fill: parent

                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked:
                    {
                        closeMessageWindow()
                    }
                }

                Rectangle
                {
                    id: sVG_2

                    anchors.centerIn: parent

                    height: 14
                    width: 14

                    clip: true
                    color: "transparent"

                    scale: clickAreaCloseMessageBox.containsMouse ? 1.5 : 1.0

                    Behavior on scale
                    {
                        NumberAnimation
                        {
                            duration: 150
                            easing.type: Easing.InOutQuad
                        }
                    }

                    Shape
                    {
                        id: _vector_5

                        x: 3.50
                        y: 3.50

                        height: 7
                        width: 7

                        ShapePath
                        {
                            id: _vector_5_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#ffffff"
                            strokeWidth: 1.17

                            PathSvg
                            {
                                id: _vector_5_ShapePath0_PathSvg0

                                path: "M 7 0 L 0 7"
                            }
                        }
                    }

                    Shape
                    {
                        id: _vector_6

                        x: 3.50
                        y: 3.50

                        height: 7
                        width: 7

                        ShapePath
                        {
                            id: _vector_6_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#ffffff"
                            strokeWidth: 1.17

                            PathSvg
                            {
                                id: _vector_6_ShapePath0_PathSvg0

                                path: "M 0 0 L 7 7"
                            }
                        }
                    }
                }
            }
        }
        }
    }

    Rectangle
    {
        id: recipient_background

        y: 38

        width: 420
        height: 36

        clip: true
        color: "#fcf3e6"
        border.color: "#e5e7eb"
        border.width: 1

        Rectangle
        {
            id: recipient_container
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            height: 19
            width: 396

            clip: true
            color: "transparent"

            TextField
            {
                id: recipient_input
                anchors.fill: parent
                color: "#1f2937"
                font.pixelSize: 14
                font.family: "Segoe UI"
                placeholderText: "To"
                placeholderTextColor: "#99a1af"
                background: Item {}
                leftPadding: 0
                topPadding: 0
                bottomPadding: 0

                cursorDelegate: Item {}

                Rectangle
                {
                    id: custom_cursor_for_recipient_container
                    width: 1
                    color: "#1f2937"

                    height: parent.font.pixelSize
                    anchors.verticalCenter: parent.verticalCenter

                    x: parent.cursorRectangle.x
                    visible: parent.activeFocus

                    Behavior on x
                    {
                        NumberAnimation
                        {
                            duration: 80
                            easing.type: Easing.OutCubic
                        }
                    }
                    SequentialAnimation on opacity
                    {
                        running: parent.activeFocus
                        loops: Animation.Infinite
                        NumberAnimation { to: 0; duration: 400; easing.type: Easing.InOutSine }
                        NumberAnimation { to: 1; duration: 400; easing.type: Easing.InOutSine }
                    }
                }
            }
        }
    }

    Rectangle
    {
        id: subject_background

        y: 72

        width: 420
        height: 36

        clip: true
        color: "#fcf3e6"
        border.color: "#e5e7eb"
        border.width: 1

        Rectangle
        {
            id: subject_container

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            height: 19

            clip: true
            color: "transparent"

            TextField
            {
                id: subject_input
                anchors.fill: parent
                color: "#1f2937"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal

                placeholderText: "Subject"
                placeholderTextColor: "#99a1af"
                background: Item {}

                leftPadding: 0
                topPadding: 0
                bottomPadding: 0

                cursorDelegate: Item {}

                Rectangle
                {
                    id: custom_cursor_for_subject_container
                    width: 1
                    color: "#1f2937"
                    height: parent.font.pixelSize
                    anchors.verticalCenter: parent.verticalCenter
                    x: parent.cursorRectangle.x
                    visible: parent.activeFocus
                    Behavior on x
                    {
                        NumberAnimation
                        {
                            duration: 80
                            easing.type: Easing.OutCubic
                        }
                    }
                    SequentialAnimation on opacity
                    {
                        loops: Animation.Infinite
                        NumberAnimation { to: 0; duration: 500; easing.type: Easing.InOutSine }
                        NumberAnimation { to: 1; duration: 500; easing.type: Easing.InOutSine }
                    }
                }
            }
        }
    }


    Rectangle
    {
        id: textarea

        y: 119

        height: 236.40
        width: 420
        radius: 14

        clip: true
        color: "#fcf3e6"

        Rectangle {
            id: container

            anchors.fill: parent
            anchors.margins: 12

            color: "transparent"

            TextArea {
                id: message_input
                anchors.fill: parent

                color: "#1f2937"
                font.family: "Segoe UI"
                placeholderText: "Write your message..."
                placeholderTextColor: "#99a1af"
                wrapMode: Text.Wrap

                background: Item {}
                leftPadding: 0
                topPadding: 0
                bottomPadding: 0

                cursorDelegate: Item {}

                Rectangle {
                    id: custom_cursor_for_message_input
                    width: 1
                    color: "#1f2937"

                    height: message_input.cursorRectangle.height

                    x: message_input.cursorRectangle.x
                    y: message_input.cursorRectangle.y

                    visible: message_input.activeFocus

                    // Smooth right/left movement
                    Behavior on x {
                        NumberAnimation {
                            duration: 80
                            easing.type: Easing.OutCubic
                        }
                    }

                    // Smooth UP/DOWN movement
                    Behavior on y {
                        NumberAnimation {
                            duration: 80
                            easing.type: Easing.OutCubic
                        }
                    }

                    // Smooth flashing
                    SequentialAnimation on opacity {
                        loops: Animation.Infinite
                        NumberAnimation { to: 0; duration: 500; easing.type: Easing.InOutSine }
                        NumberAnimation { to: 1; duration: 500; easing.type: Easing.InOutSine }
                    }
                }
            }
        }
    }

    Rectangle
    {
        id: horizontalBorder

        y: 340

        Rectangle {
            id: buttonToSend

            x: 12
            y: 8.80

            height: 32
            width: 63
            bottomLeftRadius: 14
            bottomRightRadius: 14
            color:"#155dfc"

            scale: mouseAreaToSend.containsMouse ? 1.1 : 1.0
            Behavior on scale {
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }
            radius: 26843500
            ToolTip {
                id: popup_test
                text: subject_input.text + " " + recipient_input.text + " " + message_input.text
                visible: false
                timeout: 2000
            }
            MouseArea
            {
                id:mouseAreaToSend
                anchors.fill:parent
                hoverEnabled: true
                onPressed:
                {
                    popup_test.visible = true
                    emailsModel.AddData(false, true, false, subject_input.text, "no", recipient_input.text, message_input.text, "no")
                }
            }

            Text {
                id: send

                x: 16
                y: 6

                height: 20
                width: 32

                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignHCenter
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                scale: mouseAreaToSend.containsMouse ? 1.1 : 1.0
                text: "Send"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
            }
        }

        Rectangle {
            id: buttonToDelete

            x: 83
            y: 10.80

            height: 28
            width: 28

            color: "transparent"
            radius: 4
            scale: mouseAreaToDelete.containsMouse ? 1.3 : 1.0
            Behavior on scale {
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }
            MouseArea
            {
                id:mouseAreaToDelete
                anchors.fill:parent
                hoverEnabled: true
                onPressed:
                {
                    message_input.clear()
                    recipient_input.clear()
                    subject_input.clear()
                }
            }

            Rectangle {
                id: sVG_3

                x: 6
                y: 6

                height: 16
                width: 16

                clip: true
                color: "transparent"

                Shape {
                    id: _vector_7

                    x: 2
                    y: 4

                    height: 0
                    width: 12

                    ShapePath {
                        id: _vector_7_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_7_ShapePath0_PathSvg0

                            path: "M 0 0 L 12 0"
                        }
                    }
                }
                Shape {
                    id: _vector_8

                    x: 3.33
                    y: 4

                    height: 10.67
                    width: 9.33

                    ShapePath {
                        id: _vector_8_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_8_ShapePath0_PathSvg0

                            path: "M 9.333333969116211 0 L 9.333333969116211 9.333333611488342 C 9.333333969116211 10.000000298023224 8.666667257036481 10.666666984558105 8.000000544956752 10.666666984558105 L 1.3333334241594588 10.666666984558105 C 0.6666667120797294 10.666666984558105 0 10.000000298023224 0 9.333333611488342 L 0 0"
                        }
                    }
                }
                Shape {
                    id: _vector_9

                    x: 5.33
                    y: 1.33

                    height: 2.67
                    width: 5.33

                    ShapePath {
                        id: _vector_9_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_9_ShapePath0_PathSvg0

                            path: "M 0 2.6666667461395264 L 0 1.3333333730697632 C 0 0.6666666865348816 0.6666666865348816 0 1.3333333730697632 0 L 4.0000001192092896 0 C 4.666666805744171 0 5.333333492279053 0.6666666865348816 5.333333492279053 1.3333333730697632 L 5.333333492279053 2.6666667461395264"
                        }
                    }
                }
                Shape {
                    id: _vector_10

                    x: 6.67
                    y: 7.33

                    height: 4
                    width: 0

                    ShapePath {
                        id: _vector_10_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_10_ShapePath0_PathSvg0

                            path: "M 0 0 L 0 4"
                        }
                    }
                }
                Shape {
                    id: _vector_11

                    x: 9.33
                    y: 7.33

                    height: 4
                    width: 0

                    ShapePath {
                        id: _vector_11_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_11_ShapePath0_PathSvg0

                            path: "M 0 0 L 0 4"
                        }
                    }
                }
            }
        }
    }
}
