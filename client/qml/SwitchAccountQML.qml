import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    implicitWidth: 230
    implicitHeight: 120
    radius: 10
    color: "#ffffff"
    border.color: "#e5e7eb"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        Text {
            Layout.fillWidth: true
            text: mailClient.senderName
            color: "#101828"
            font.bold: true
            elide: Text.ElideRight
        }

        Text {
            Layout.fillWidth: true
            text: mailClient.senderAddress
            color: "#6a7282"
            elide: Text.ElideRight
        }

        Text {
            Layout.fillWidth: true
            text: mailClient.serverHost + ":" + mailClient.serverPort
            color: "#6a7282"
            font.pixelSize: 12
            elide: Text.ElideRight
        }
    }
}
