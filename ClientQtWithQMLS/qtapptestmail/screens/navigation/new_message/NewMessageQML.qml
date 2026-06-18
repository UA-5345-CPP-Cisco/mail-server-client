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

        Rectangle
        {
            id: drag_container

            x: 12
            y: 9

            height: 20
            width: 306

            clip: true
            color: "transparent"

            Text
            {
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

        Rectangle
        {
            id: windowControlsWrapper
            x: 320
            y: 8
            width: 140
            height: 45
            color: "transparent"

            MouseArea
            {
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
                    Rectangle
                    {
                        id: sVG

                        x: 4
                        y: 4

                        height: 14
                        width: 14

                        clip: true
                        color: "transparent"
                        scale: clickAreaMinimizeWindow.containsMouse ? 1.5 : 1.0

                        Behavior on scale
                        {
                            NumberAnimation
                            {
                                duration: 150
                                easing.type: Easing.InOutQuad
                            }
                        }

                        Image
                        {
                           source: "qrc:/pngs/assets/ic_minimize_window.svg"
                           width: 14
                           height: 14
                           sourceSize.width: 120
                           sourceSize.height: 120
                           fillMode: Image.PreserveAspectFit
                           anchors.centerIn: parent
                        }
                    }
                }

                //button to maximize window
                Rectangle
                {
                    id: buttonToMaximizeWindow

                    height: 22
                    width: 22
                    radius: 4

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

                        Image
                        {
                           source: "qrc:/pngs/assets/ic_extend_window.svg"
                           width: 14
                           height: 14
                           sourceSize.width: 120
                           sourceSize.height: 120
                           fillMode: Image.PreserveAspectFit
                           anchors.centerIn: parent
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
                        if(subject_input.text.trim() === "" && recipient_input.text.trim() === "" && message_input.text.trim() === "")
                        {
                            closeMessageWindow()
                        }
                        else
                        {
                            let subject_text = subject_input.text.trim();
                            let recipient_text = recipient_input.text.trim();
                            let message_text = message_input.text.trim();

                            messageComposer.SaveDraft(
                                currentUser.username,
                                currentUser.email,
                                recipient_text,
                                subject_text,
                                message_text
                            );
                            emailsModel.AddData(false, false, true, subject_text, currentUser.username, recipient_text, message_text, "");
                            closeMessageWindow()
                        }
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

                    Image
                    {
                       source: "qrc:/pngs/assets/ic_close_window.svg"
                       width: 14
                       height: 14
                       sourceSize.width: 120
                       sourceSize.height: 120
                       fillMode: Image.PreserveAspectFit
                       anchors.centerIn: parent
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

        Rectangle
        {
            id: container

            anchors.fill: parent
            anchors.margins: 12

            color: "transparent"

            TextArea
            {
                id: message_input
                anchors.fill: parent

                color: "#1f2937"
                font.family: "Segoe UI"
                placeholderText: "Write your message..."
                placeholderTextColor: "#99a1af"
                wrapMode: Text.Wrap
                font.pixelSize: 14
                background: Item {}
                leftPadding: 0
                topPadding: 0
                bottomPadding: 0

                cursorDelegate: Item {}

                Rectangle
                {
                    id: custom_cursor_for_message_input
                    width: 1
                    color: "#1f2937"

                    height: message_input.cursorRectangle.height

                    x: message_input.cursorRectangle.x
                    y: message_input.cursorRectangle.y

                    visible: message_input.activeFocus

                    // Smooth right/left movement
                    Behavior on x
                    {
                        NumberAnimation
                        {
                            duration: 80
                            easing.type: Easing.OutCubic
                        }
                    }

                    // Smooth UP/DOWN movement
                    Behavior on y
                    {
                        NumberAnimation
                        {
                            duration: 80
                            easing.type: Easing.OutCubic
                        }
                    }

                    // Smooth flashing
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
        id: horizontalBorder

        y: 340

        Rectangle
        {
            id: buttonToSend

            x: 12
            y: 8.80

            height: 32
            width: 63
            color:"#155dfc"

            scale: mouseAreaToSend.containsMouse ? 1.1 : 1.0
            Behavior on scale
            {
                NumberAnimation
                {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }
            radius: 14
            ToolTip
            {
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
                    if(recipient_input.text === "")
                    {
                        recipient_input.text = "Enter Recipient!"
                    }
                    else
                    {

                        let subject_text = (subject_input.text.trim() === "") ? "empty" : subject_input.text;
                        let message_text = (message_input.text.trim() === "") ? "empty" : message_input.text;

                        if (messageComposer.SendMessage(
                                currentUser.username,
                                currentUser.email,
                                recipient_input.text.trim(),
                                subject_text,
                                message_text))
                        {
                            emailsModel.AddData(false, true, false, subject_text, currentUser.username, recipient_input.text, message_text, "");
                        }
                        message_input.clear()
                        recipient_input.clear()
                        subject_input.clear()
                    }
                }
            }

            Text
            {
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
        Rectangle
        {
            id: buttonToDelete

            x: 83
            y: 10.80

            height: 28
            width: 28

            color: "transparent"
            radius: 4
            scale: mouseAreaToDelete.containsMouse ? 1.3 : 1.0
            Behavior on scale
            {
                NumberAnimation
                {
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

            Rectangle
            {
                id: sVG_3

                x: 6
                y: 6

                height: 16
                width: 16

                clip: true
                color: "transparent"

                Image
                {
                   source: "qrc:/pngs/assets/ic_bin.svg"
                   width: 16
                   height: 16
                   sourceSize.width: 120
                   sourceSize.height: 120
                   fillMode: Image.PreserveAspectFit
                   anchors.centerIn: parent
                }
            }
        }
    }
}
