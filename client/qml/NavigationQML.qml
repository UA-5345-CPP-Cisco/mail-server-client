import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: navigationQML
    color: "#ffffff"

    signal composeRequested()
    signal settingsRequested()

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: "#ffffff"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8

                Rectangle {
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 32
                    radius: 16
                    color: "#2b7fff"

                    Text {
                        anchors.centerIn: parent
                        text: mailClient.senderName.length > 0 ? mailClient.senderName.charAt(0).toUpperCase() : "A"
                        color: "#ffffff"
                        font.pixelSize: 14
                        font.bold: true
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 0

                    Text {
                        Layout.fillWidth: true
                        text: mailClient.senderName.length > 0 ? mailClient.senderName : "NameHolder"
                        color: "#101828"
                        font.pixelSize: 14
                        elide: Text.ElideRight
                    }

                    Text {
                        Layout.fillWidth: true
                        text: mailClient.senderAddress.length > 0 ? mailClient.senderAddress : "EmailHolder"
                        color: "#6a7282"
                        font.pixelSize: 12
                        elide: Text.ElideRight
                    }
                }

                Image {
                    Layout.preferredWidth: 16
                    Layout.preferredHeight: 16
                    source: "assets/ArrowDownToPullEmails.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 1
                color: "#e5e7eb"
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 76

            Button {
                anchors.fill: parent
                anchors.margins: 16
                text: "Compose"
                onClicked: navigationQML.composeRequested()
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 8
            spacing: 4

            Button { Layout.fillWidth: true; text: "Inbox"; flat: true }
            Button { Layout.fillWidth: true; text: "Starred"; flat: true }
            Button { Layout.fillWidth: true; text: "Sent"; flat: true }
            Button { Layout.fillWidth: true; text: "Drafts"; flat: true }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: "#ffffff"

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: 1
                color: "#e5e7eb"
            }

            Button {
                anchors.centerIn: parent
                width: parent.width - 24
                height: 36
                flat: true
                text: "Settings"
                onClicked: navigationQML.settingsRequested()
            }
        }
    }
}
