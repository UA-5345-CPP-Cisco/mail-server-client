import QtQuick
import QtQuick.Layouts

Rectangle {
    id: listItem
    property string theme: "Inbox"
    property string name: "Sender"
    property string preview: "Preview text"
    property string time: "10:20"

    implicitHeight: 76
    color: "#ffffff"

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 1
        color: "#e5e7eb"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 4

        RowLayout {
            Layout.fillWidth: true

            Text {
                Layout.fillWidth: true
                text: theme
                color: "#101828"
                font.pixelSize: 14
                font.bold: true
                elide: Text.ElideRight
            }

            Text {
                text: time
                color: "#6a7282"
                font.pixelSize: 12
            }
        }

        Text {
            Layout.fillWidth: true
            text: name
            color: "#4a5565"
            font.pixelSize: 13
            elide: Text.ElideRight
        }

        Text {
            Layout.fillWidth: true
            text: preview
            color: "#6a7282"
            font.pixelSize: 12
            elide: Text.ElideRight
        }
    }
}
