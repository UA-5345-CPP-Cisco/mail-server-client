import QtQuick
import QtQuick.Shapes
import QtQuick.Controls

Rectangle
{
    id: newMessageQml
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
            newMessageQml.forceActiveFocus()
        }
    }

    //header
    Rectangle {
        id: headerNewMessage
        height: 38
        width: parent.width
        topLeftRadius: 14
        topRightRadius: 14
        color: "#1e2939"

        MouseArea {
            id: headerDragClickArea
            anchors.fill: parent
            cursorShape: Qt.OpenHandCursor
            drag.target: newMessageQml
            drag.axis: Drag.XAndYAxis

            onPressed: cursorShape = Qt.ClosedHandCursor
            onReleased: cursorShape = Qt.OpenHandCursor
        }

        Rectangle {
            id: newMessageTitleWrapper
            x: 12
            y: 9
            height: 20
            width: 250
            clip: true
            color: "transparent"

            Text {
                id: newMessageTitleText
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
        Rectangle {
            id: windowControlsWrapper
            x: 320
            y: 8
            width: 140
            height: 45
            color: "transparent"

            MouseArea {
                id: clickBlockerClickArea
                anchors.fill: parent
                anchors.leftMargin: -8
                anchors.rightMargin: 8
                anchors.topMargin: -8
                anchors.bottomMargin: 8
                hoverEnabled: true
                cursorShape: Qt.ArrowCursor

            }

            Row {
                id: newMessageControlsRow
                x: 340
                y: 8
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: -8
                anchors.topMargin: 0
                anchors.bottomMargin: 0
                spacing: 8

                //button to minimize window
                Rectangle {
                    id: buttonToMinimizeWindow
                    height: 22
                    width: 22

                    color: minimizeWindowClickArea.pressed ? "#ffdede" : "transparent"
                    radius: 4

                    MouseArea {
                        id: minimizeWindowClickArea
                        anchors.fill: parent

                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: {
                            // MinimizeWindow
                        }
                    }
                    Rectangle {
                        id: minimizeWindowIcon

                        x: 4
                        y: 4

                        height: 14
                        width: 14

                        clip: true
                        color: "transparent"
                        scale: minimizeWindowClickArea.containsMouse ? 1.5 : 1.0

                        Behavior on scale {
                            NumberAnimation {
                                duration: 150
                                easing.type: Easing.InOutQuad
                            }
                        }

                        Image {
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
                Rectangle {
                    id: buttonToMaximizeWindow

                    height: 22
                    width: 22
                    radius: 4

                    color: maximizeWindowClickArea.pressed ? "#ffdede" : "transparent"

                    MouseArea {
                        id: maximizeWindowClickArea
                        anchors.fill: parent

                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: {
                            //MaximizeWindow
                        }
                    }

                    Rectangle {
                        id: maximizeWindowIcon
                        x: 4
                        y: 4
                        height: 14
                        width: 14
                        clip: true
                        color: "transparent"
                        scale: maximizeWindowClickArea.containsMouse ? 1.5 : 1.0

                        Behavior on scale {
                            NumberAnimation {
                                duration: 150
                                easing.type: Easing.InOutQuad
                            }
                        }

                        Image {
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

                    color: closeMessageBoxClickArea.pressed ? "#ffdede" : "transparent"

                    MouseArea
                    {
                        id: closeMessageBoxClickArea
                        anchors.fill: parent

                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked:
                        {
                            if (subjectTextField.text.trim() === "" && recipientTextField.text.trim() === "" && messageBodyTextField.text.trim() === "")
                            {
                                closeMessageWindow()
                            }
                            else
                            {
                                let subject_text = subjectTextField.text.trim()
                                let recipient_text = recipientTextField.text.trim()
                                let message_text = messageBodyTextField.text.trim()

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
                                } else
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
                        id: closeMessageBoxIcon

                        anchors.centerIn: parent

                        height: 14
                        width: 14

                        clip: true
                        color: "transparent"

                        scale: closeMessageBoxClickArea.containsMouse ? 1.5 : 1.0

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

    //recipient
    Rectangle
    {
        id: recipientWrapper
        y: 38
        width: parent.width
        height: 36
        clip: true
        color: "#fcf3e6"
        border.color: "#e5e7eb"
        border.width: 1

        Rectangle
        {
            id: recipientContainer
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            height: 19
            color: "transparent"

            TextField
            {
                id: recipientTextField
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
                    id: customCursorRecipient
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
        id: subjectWrapper
        y: 72
        width: parent.width
        height: 36
        clip: true
        color: "#fcf3e6"
        border.color: "#e5e7eb"
        border.width: 1

        Rectangle
        {
            id: subjectContainer
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            height: 19
            color: "transparent"

            TextField
            {
                id: subjectTextField
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
                    id: custonCursorSubject
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

    //message body field
    Rectangle
    {
        id: messageBodyWrapper
        y: 107
        height: 233
        width: parent.width
        clip: true
        color: "#fcf3e6"
        border.color: "#e5e7eb"
        border.width: 1

        Rectangle
        {
            id: messageBodyContainer
            anchors.fill: parent
            anchors.margins: 12
            color: "transparent"

            TextArea
            {
                id: messageBodyTextField
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
                    id: customCursorMessageBody
                    width: 1
                    color: "#1f2937"
                    height: messageBodyTextField.cursorRectangle.height
                    x: messageBodyTextField.cursorRectangle.x
                    y: messageBodyTextField.cursorRectangle.y
                    visible: messageBodyTextField.activeFocus

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
    //footer
    Rectangle
    {
        id: footerNavigation
        y: 340
        width: parent.width
        height: 58
        color: "transparent"

        // Send
        Rectangle
        {
            id: buttonToSentMessage
            x: 12
            anchors.verticalCenter: parent.verticalCenter
            height: 32
            width: 63
            color: "#155dfc"
            radius: 14
            scale: sentClickArea.containsMouse ? 1.05 : 1.0

            Behavior on scale
            {
                NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
            }


            MouseArea
            {
                id: sentClickArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked:
                {
                    if (recipientTextField.text === "")
                    {
                        recipientTextField.text = "Enter Recipient!"
                    }
                    else
                    {
                        let recipient_text = recipientTextField.text.trim() === "" ? "empty" : recipientTextField.text
                        let subject_text   = subjectTextField.text.trim()   === "" ? "empty" : subjectTextField.text
                        let message_text   = messageBodyTextField.text.trim()   === "" ? "empty" : messageBodyTextField.text

                        if(recipient_text === "inboxtest")
                        {
                            if (!isDraft && MessageComposer.SendMessage(
                                        CurrentUser.username,
                                        CurrentUser.email,
                                        recipientTextField.text.trim(),
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
                                    recipientTextField.text.trim(),
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
                        messageBodyTextField.clear()
                        recipientTextField.clear()
                        subjectTextField.clear()
                    }
                }
            }

            Text
            {
                id: sentButtonText
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
            id: buttonToDelete
            x: 85
            anchors.verticalCenter: parent.verticalCenter
            height: 28
            width: 28
            color: "transparent"
            radius: 4
            scale: deleteClickArea.containsMouse ? 1.2 : 1.0

            Behavior on scale
            {
                NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
            }

            MouseArea
            {
                id: deleteClickArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked:
                {
                    messageBodyTextField.clear()
                    recipientTextField.clear()
                    subjectTextField.clear()
                }
            }

            Rectangle
            {
                id: deleteButtonIcon
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
