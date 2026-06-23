import QtQuick
import QtQuick.Shapes
import QtQuick.Controls

Rectangle
{
    id: new_message_qml
    implicitHeight: 398
    implicitWidth: 420
    border.color: "#e5e7eb"

    property bool isDraft: false
    property string newIndex: ""
    property string newTitle: "New Message"
    property string newSubject: ""
    property string newRecipient: ""
    property string newText: ""

    signal draftFinished(string index, string subject, string recipient, string text)
    signal draftChanged(string index, string subject, string recipient, string text)

    clip: true
    color: "#fcf3e6"
    radius: 14

    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            new_message_qml.forceActiveFocus()
        }
    }

    //header
    Rectangle
    {
        id: header_bar
        height: 38
        width: parent.width
        topLeftRadius: 14
        topRightRadius: 14
        color: "#1e2939"

        MouseArea
        {
            anchors.fill: parent
            cursorShape: Qt.OpenHandCursor
            drag.target: new_message_qml
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
            width: 250
            clip: true
            color: "transparent"

            Text
            {
                id: label_title
                anchors.fill: parent
                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                text: newTitle
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }
        }

        // Wrapper
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
            anchors.leftMargin: -8
            anchors.rightMargin: 8
            anchors.topMargin: -8
            anchors.bottomMargin: 8
            hoverEnabled: true
            cursorShape: Qt.ArrowCursor

        }

        Row
        {
            id:new_message_controls
            x: 340
            y: 8
            anchors.fill: parent
            anchors.leftMargin: 8
            anchors.rightMargin: -8
            anchors.topMargin: 0
            anchors.bottomMargin: 0
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
                        if (subject_input.text.trim() === "" && recipient_input.text.trim() === "" && message_input.text.trim() === "")
                        {
                            closeMessageWindow()
                        }
                        else
                        {
                            let subject_text = subject_input.text.trim()
                            let recipient_text = recipient_input.text.trim()
                            let message_text = message_input.text.trim()

                            MessageComposer.SaveDraft(
                                CurrentUser.username,
                                CurrentUser.email,
                                recipient_text,
                                subject_text,
                                message_text
                            )

                            if (isDraft)
                            {
                                draftChanged(newIndex, subject_text, recipient_text, message_text)
                            }
                            else
                            {
                                emailsModel.AddData(
                                    false, false, true,
                                    subject_text, CurrentUser.username,
                                    recipient_text, message_text, ""
                                )
                            }
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




    //recipient
    Rectangle
    {
        id: recipient_background
        y: 38
        width: parent.width
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
                text: newRecipient
                cursorDelegate: Item {}

                Rectangle
                {
                    id: custom_cursor_recipient
                    width: 1
                    color: "#1f2937"
                    height: parent.font.pixelSize
                    anchors.verticalCenter: parent.verticalCenter
                    x: parent.cursorRectangle.x
                    visible: parent.activeFocus

                    Behavior on x
                    {
                        NumberAnimation { duration: 80; easing.type: Easing.OutCubic }
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

    //subject field
    Rectangle
    {
        id: subject_background
        y: 72
        width: parent.width
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
                text: newSubject
                leftPadding: 0
                topPadding: 0
                bottomPadding: 0
                cursorDelegate: Item {}

                Rectangle
                {
                    id: custom_cursor_subject
                    width: 1
                    color: "#1f2937"
                    height: parent.font.pixelSize
                    anchors.verticalCenter: parent.verticalCenter
                    x: parent.cursorRectangle.x
                    visible: parent.activeFocus

                    Behavior on x
                    {
                        NumberAnimation { duration: 80; easing.type: Easing.OutCubic }
                    }
                    SequentialAnimation on opacity
                    {
                        running: parent.activeFocus
                        loops: Animation.Infinite
                        NumberAnimation { to: 0; duration: 500; easing.type: Easing.InOutSine }
                        NumberAnimation { to: 1; duration: 500; easing.type: Easing.InOutSine }
                    }
                }
            }
        }
    }
    //textarea
    Rectangle
    {
        id: text_area_background
        y: 107
        height: 233
        width: parent.width
        clip: true
        color: "#fcf3e6"
        border.color: "#e5e7eb"
        border.width: 1

        Rectangle
        {
            id: text_container
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
                text: newText
                cursorDelegate: Item {}

                Rectangle
                {
                    id: custom_cursor_message
                    width: 1
                    color: "#1f2937"
                    height: message_input.cursorRectangle.height
                    x: message_input.cursorRectangle.x
                    y: message_input.cursorRectangle.y
                    visible: message_input.activeFocus

                    Behavior on x
                    {
                        NumberAnimation { duration: 80; easing.type: Easing.OutCubic }
                    }
                    Behavior on y
                    {
                        NumberAnimation { duration: 80; easing.type: Easing.OutCubic }
                    }
                    SequentialAnimation on opacity
                    {
                        running: parent.activeFocus
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
        id: bottom_action_bar
        y: 340
        width: parent.width
        height: 58
        color: "transparent"

        // Send
        Rectangle
        {
            id: button_send
            x: 12
            anchors.verticalCenter: parent.verticalCenter
            height: 32
            width: 63
            color: "#155dfc"
            radius: 14
            scale: mouse_area_send.containsMouse ? 1.05 : 1.0

            Behavior on scale
            {
                NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
            }

            ToolTip
            {
                id: popup_test
                text: subject_input.text + " " + recipient_input.text + " " + message_input.text
                visible: false
                timeout: 2000
            }

            MouseArea
            {
                id: mouse_area_send
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked:
                {
                    if (recipient_input.text === "")
                    {
                        recipient_input.text = "Enter Recipient!"
                    }
                    else
                    {
                        let recipient_text = recipient_input.text.trim() === "" ? "empty" : recipient_input.text
                        let subject_text   = subject_input.text.trim()   === "" ? "empty" : subject_input.text
                        let message_text   = message_input.text.trim()   === "" ? "empty" : message_input.text

                        if(recipient_text === "inboxtest")
                        {
                            if (!isDraft && MessageComposer.SendMessage(
                                        CurrentUser.username,
                                        CurrentUser.email,
                                        recipient_input.text.trim(),
                                        subject_text,
                                        message_text,
                                        true))
                            {
                                emailsModel.AddData(
                                    true, false, false,
                                    subject_text, CurrentUser.username,
                                    recipient_text, message_text, ""
                                )
                            }
                        }
                        else if (!isDraft && MessageComposer.SendMessage(
                                    CurrentUser.username,
                                    CurrentUser.email,
                                    recipient_input.text.trim(),
                                    subject_text,
                                    message_text,
                                    false))
                        {
                            emailsModel.AddData(
                                false, true, false,
                                subject_text, CurrentUser.username,
                                recipient_text, message_text, ""
                            )
                        }
                        else
                        {
                            draftFinished(newIndex, subject_text, recipient_text, message_text)
                        }
                        message_input.clear()
                        recipient_input.clear()
                        subject_input.clear()
                    }
                }
            }

            Text
            {
                id: label_send
                anchors.centerIn: parent
                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                text: "Send"
                textFormat: Text.PlainText
            }
        }

        // Delete
        Rectangle
        {
            id: button_delete
            x: 85
            anchors.verticalCenter: parent.verticalCenter
            height: 28
            width: 28
            color: "transparent"
            radius: 4
            scale: mouse_area_delete.containsMouse ? 1.2 : 1.0

            Behavior on scale
            {
                NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
            }

            MouseArea
            {
                id: mouse_area_delete
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked:
                {
                    message_input.clear()
                    recipient_input.clear()
                    subject_input.clear()
                }
            }

            Rectangle
            {
                id: svg_bin
                anchors.centerIn: parent
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
}
