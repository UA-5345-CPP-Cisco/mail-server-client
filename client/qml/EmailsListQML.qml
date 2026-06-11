import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: emailsListQML
    color: "#ffffff"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 70
            color: "#ffffff"

            TextField {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 16
                height: 38
                placeholderText: "Search mail"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 34
            color: "#f9fafb"

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                text: "Inbox"
                color: "#6a7282"
                font.pixelSize: 12
                font.bold: true
                font.capitalization: Font.AllUppercase
            }

            Text {
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                text: "3"
                color: "#6a7282"
                font.pixelSize: 12
            }
        }

        ListItem {
            Layout.fillWidth: true
            theme: "Welcome"
            name: "Mail Client"
            preview: "Use Compose to send through the local SMTP server."
            time: "Now"
        }

        ListItem {
            Layout.fillWidth: true
            theme: "Server"
            name: "localhost:2525"
            preview: "Default server settings are ready for the sibling server app."
            time: "Local"
        }

        ListItem {
            Layout.fillWidth: true
            theme: "Draft"
            name: "New message"
            preview: "Messages are sent through mail_client_core."
            time: "GUI"
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    Rectangle {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 1
        color: "#e5e7eb"
    }

    Rectangle {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 1
        color: "#e5e7eb"
    }
}
