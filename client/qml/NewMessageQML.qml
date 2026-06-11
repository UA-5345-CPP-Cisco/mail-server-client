import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: newMessageQML
    implicitWidth: 420
    implicitHeight: 430
    color: "#fcf3e6"
    radius: 14
    border.color: "#e5e7eb"
    clip: true

    Connections {
        target: mailClient
        function onOperationSucceeded(message) {
            if (message.indexOf("Message sent") === 0) {
                closeMessageWindow()
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: titlebar
            Layout.fillWidth: true
            Layout.preferredHeight: 38
            color: "#1e2939"

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.OpenHandCursor
                drag.target: newMessageQML
                drag.axis: Drag.XAndYAxis
                onPressed: cursorShape = Qt.ClosedHandCursor
                onReleased: cursorShape = Qt.OpenHandCursor
            }

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                text: "New Message"
                color: "#ffffff"
                font.pixelSize: 14
            }

            Button {
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                width: 28
                height: 28
                text: "x"
                flat: true
                onClicked: closeMessageWindow()
            }
        }

        TextField {
            id: toField
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            placeholderText: "To"
            background: Rectangle {
                color: "#fcf3e6"
                border.color: "#e5e7eb"
            }
        }

        TextField {
            id: subjectField
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            placeholderText: "Subject"
            background: Rectangle {
                color: "#fcf3e6"
                border.color: "#e5e7eb"
            }
        }

        TextArea {
            id: bodyField
            Layout.fillWidth: true
            Layout.fillHeight: true
            placeholderText: "Write your message..."
            wrapMode: TextArea.Wrap
            background: Rectangle {
                color: "#fcf3e6"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 54
            color: "#fcf3e6"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                spacing: 8

                Button {
                    text: mailClient.busy ? "Sending..." : "Send"
                    enabled: !mailClient.busy
                    onClicked: mailClient.sendMessage(toField.text, subjectField.text, bodyField.text)
                }

                Button {
                    text: "Delete"
                    flat: true
                    enabled: !mailClient.busy
                    onClicked: {
                        toField.clear()
                        subjectField.clear()
                        bodyField.clear()
                    }
                }

                Text {
                    Layout.fillWidth: true
                    text: mailClient.statusMessage
                    color: "#6a7282"
                    font.pixelSize: 12
                    elide: Text.ElideRight
                }
            }
        }
    }
}
