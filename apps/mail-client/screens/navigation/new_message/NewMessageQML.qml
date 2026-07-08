import QtQuick
import QtQuick.Shapes
import QtQuick.Controls

Rectangle {
    id: newMessageQml

    property bool isDraft: false
    property bool isReply: false
    property bool isForward: false
    property string newIndex: ""
    property string newRecipient: ""
    property string newSubject: ""
    property string newText: ""
    property string newTitle: "New Message"

    signal draftChanged(string index, string subject, string recipient, string text)
    signal draftFinished(string index, string subject, string recipient, string text)

    border.color: Color.outline
    clip: true
    color: "#fcf3e6"
    implicitHeight: 398
    implicitWidth: 420
    radius: 14

    MouseArea {
        anchors.fill: parent

        onClicked: {
            newMessageQml.forceActiveFocus();
        }
    }

    //header
    Rectangle {
        id: headerNewMessage

        color: "#1e2939"
        height: 38
        topLeftRadius: 14
        topRightRadius: 14
        width: parent.width

        MouseArea {
            id: headerDragClickArea

            anchors.fill: parent
            cursorShape: Qt.OpenHandCursor
            drag.axis: Drag.XAndYAxis
            drag.target: newMessageQml

            onPressed: cursorShape = Qt.ClosedHandCursor
            onReleased: cursorShape = Qt.OpenHandCursor
        }
        Rectangle {
            id: newMessageTitleWrapper

            clip: true
            color: Color.transparent
            height: 20
            width: 250
            x: 12
            y: 9

            Text {
                id: newMessageTitleText

                anchors.fill: parent
                color: Color.background
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

            color: Color.transparent
            height: 45
            width: 140
            x: 384
            y: 8

            MouseArea {
                id: clickBlockerClickArea

                anchors.bottomMargin: 8
                anchors.fill: parent
                anchors.leftMargin: -8
                anchors.rightMargin: 8
                anchors.topMargin: -8
                cursorShape: Qt.ArrowCursor
                hoverEnabled: true
            }
            Row {
                id: newMessageControlsRow

                anchors.bottomMargin: 0
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: -8
                anchors.topMargin: 0
                spacing: 8
                x: 384
                y: 8

                //button to close window
                Rectangle {
                    id: buttonToCloseWindow

                    color: closeMessageBoxClickArea.pressed ? "#ffdede" : Color.transparent
                    height: 22
                    radius: 4
                    width: 22

                    MouseArea {
                        id: closeMessageBoxClickArea

                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true

                        onClicked: {
                            if ((subjectTextField.text.trim() === "" && recipientTextField.text.trim() === "" && messageBodyTextField.text.trim() === "") || isDraft || isReply || isForward) {
                                closeMessageWindow();
                            } else {
                                let subject_text = subjectTextField.text.trim();
                                let recipient_text = recipientTextField.text.trim();
                                let message_text = messageBodyTextField.text.trim();

                                MessageComposer.SaveDraft(CurrentUser.username, CurrentUser.email, recipient_text, subject_text, message_text);

                                if (isDraft) {
                                    draftChanged(newIndex, subject_text, recipient_text, message_text);
                                } else {
                                    emailsModel.AddData(false, false, true, subject_text, CurrentUser.username, recipient_text, message_text, "");
                                }
                                closeMessageWindow();
                            }
                        }
                    }
                    Rectangle {
                        id: closeMessageBoxIcon

                        anchors.centerIn: parent
                        clip: true
                        color: Color.transparent
                        height: 14
                        scale: closeMessageBoxClickArea.containsMouse ? 1.5 : 1.0
                        width: 14

                        Behavior on scale {
                            NumberAnimation {
                                duration: 150
                                easing.type: Easing.InOutQuad
                            }
                        }

                        Image {
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                            height: 14
                            source: "qrc:/pngs/assets/ic_close_window.svg"
                            sourceSize.height: 120
                            sourceSize.width: 120
                            width: 14
                        }
                    }
                }
            }
        }
    }

    //recipient
    Rectangle {
        id: recipientWrapper

        border.color: Color.outline
        border.width: 1
        clip: true
        color: "#fcf3e6"
        height: 36
        width: parent.width
        y: 38

        Rectangle {
            id: recipientContainer

            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.verticalCenter: parent.verticalCenter
            color: Color.transparent
            height: 19

            TextField {
                id: recipientTextField

                anchors.fill: parent
                bottomPadding: 0
                color:Color.hover
                font.family: "Segoe UI"
                font.pixelSize: 14
                leftPadding: 0
                placeholderText: "To"
                placeholderTextColor: "#99a1af"
                text: newRecipient
                topPadding: 0

                background: Item {
                }
                cursorDelegate: Item {
                }

                Rectangle {
                    id: customCursorRecipient

                    anchors.verticalCenter: parent.verticalCenter
                    color:Color.hover
                    height: parent.font.pixelSize
                    visible: parent.activeFocus
                    width: 1
                    x: parent.cursorRectangle.x

                    SequentialAnimation on opacity {
                        loops: Animation.Infinite
                        running: parent.activeFocus

                        NumberAnimation {
                            duration: 400
                            easing.type: Easing.InOutSine
                            to: 0
                        }
                        NumberAnimation {
                            duration: 400
                            easing.type: Easing.InOutSine
                            to: 1
                        }
                    }
                    Behavior on x {
                        NumberAnimation {
                            duration: 80
                            easing.type: Easing.OutCubic
                        }
                    }
                }
            }
        }
    }

    //subject field
    Rectangle {
        id: subjectWrapper

        border.color: Color.outline
        border.width: 1
        clip: true
        color: "#fcf3e6"
        height: 36
        width: parent.width
        y: 72

        Rectangle {
            id: subjectContainer

            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.verticalCenter: parent.verticalCenter
            color: Color.transparent
            height: 19

            TextField {
                id: subjectTextField

                anchors.fill: parent
                bottomPadding: 0
                color:Color.hover
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                leftPadding: 0
                placeholderText: "Subject"
                placeholderTextColor: "#99a1af"
                text: newSubject
                topPadding: 0

                background: Item {
                }
                cursorDelegate: Item {
                }

                Rectangle {
                    id: custonCursorSubject

                    anchors.verticalCenter: parent.verticalCenter
                    color:Color.hover
                    height: parent.font.pixelSize
                    visible: parent.activeFocus
                    width: 1
                    x: parent.cursorRectangle.x

                    SequentialAnimation on opacity {
                        loops: Animation.Infinite
                        running: parent.activeFocus

                        NumberAnimation {
                            duration: 500
                            easing.type: Easing.InOutSine
                            to: 0
                        }
                        NumberAnimation {
                            duration: 500
                            easing.type: Easing.InOutSine
                            to: 1
                        }
                    }
                    Behavior on x {
                        NumberAnimation {
                            duration: 80
                            easing.type: Easing.OutCubic
                        }
                    }
                }
            }
        }
    }

    //message body field
    Rectangle {
        id: messageBodyWrapper

        border.color: Color.outline
        border.width: 1
        clip: true
        color: "#fcf3e6"
        height: 233
        width: parent.width
        y: 107

        Rectangle {
            id: messageBodyContainer

            anchors.fill: parent
            anchors.margins: 12
            color: Color.transparent

            TextArea {
                id: messageBodyTextField

                anchors.fill: parent
                bottomPadding: 0
                color:Color.hover
                font.family: "Segoe UI"
                font.pixelSize: 14
                leftPadding: 0
                placeholderText: "Write your message..."
                placeholderTextColor: "#99a1af"
                text: newText
                topPadding: 0
                wrapMode: Text.Wrap

                background: Item {
                }
                cursorDelegate: Item {
                }

                Rectangle {
                    id: customCursorMessageBody

                    color:Color.hover
                    height: messageBodyTextField.cursorRectangle.height
                    visible: messageBodyTextField.activeFocus
                    width: 1
                    x: messageBodyTextField.cursorRectangle.x
                    y: messageBodyTextField.cursorRectangle.y

                    SequentialAnimation on opacity {
                        loops: Animation.Infinite
                        running: parent.activeFocus

                        NumberAnimation {
                            duration: 500
                            easing.type: Easing.InOutSine
                            to: 0
                        }
                        NumberAnimation {
                            duration: 500
                            easing.type: Easing.InOutSine
                            to: 1
                        }
                    }
                    Behavior on x {
                        NumberAnimation {
                            duration: 80
                            easing.type: Easing.OutCubic
                        }
                    }
                    Behavior on y
                    {
                        NumberAnimation
                        {
                            duration: 80
                            easing.type: Easing.OutCubic
                        }
                    }
                }
            }
        }
    }

    //footer
    Rectangle
    {
        id: footerNavigation

        color: Color.transparent
        height: 58
        width: parent.width
        y: 340

        // Send
        Rectangle
        {
            id: buttonToSentMessage

            anchors.verticalCenter: parent.verticalCenter
            color: "#155dfc"
            height: 32
            radius: 14
            scale: sentClickArea.containsMouse ? 1.05 : 1.0
            width: 63
            x: 12

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
                id: sentClickArea

                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked:
                {
                    if (recipientTextField.text === "")
                    {
                        recipientTextField.text = "Enter Recipient!";
                    } else {
                        let recipient_text = recipientTextField.text.trim() === "" ? "empty" : recipientTextField.text;
                        let subject_text = subjectTextField.text.trim() === "" ? "empty" : subjectTextField.text;
                        let message_text = messageBodyTextField.text.trim() === "" ? "empty" : messageBodyTextField.text;

                        if (recipient_text === "inboxtest")
                        {
                            if (!isDraft &&
                                MessageComposer.SendMessage(CurrentUser.username, CurrentUser.email, recipientTextField.text.trim(), subject_text, message_text, true))
                            {
                                emailsModel.AddData(false, false, false, subject_text, CurrentUser.username, recipient_text, message_text, "", true);
                            }
                        } else if (!isDraft && MessageComposer.SendMessage(CurrentUser.username, CurrentUser.email, recipientTextField.text.trim(), subject_text, message_text, false))
                        {
                            emailsModel.AddData(false, true, false, subject_text, CurrentUser.username, recipient_text, message_text, "");
                        } else
                        {
                            draftFinished(newIndex, subject_text, recipient_text, message_text);
                        }
                        messageBodyTextField.clear();
                        recipientTextField.clear();
                        subjectTextField.clear();
                    }
                }
            }
            Text
            {
                id: sentButtonText

                anchors.centerIn: parent
                color: Color.background
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

            anchors.verticalCenter: parent.verticalCenter
            color: Color.transparent
            height: 28
            radius: 4
            scale: deleteClickArea.containsMouse ? 1.2 : 1.0
            width: 28
            x: 85

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
                id: deleteClickArea

                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked:
                {
                    messageBodyTextField.clear();
                    recipientTextField.clear();
                    subjectTextField.clear();
                }
            }
            Rectangle
            {
                id: deleteButtonIcon

                anchors.centerIn: parent
                clip: true
                color: Color.transparent
                height: 16
                width: 16

                Image
                {
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    height: 16
                    source: "qrc:/pngs/assets/ic_bin.svg"
                    sourceSize.height: 120
                    sourceSize.width: 120
                    width: 16
                }
            }
        }
    }
}
