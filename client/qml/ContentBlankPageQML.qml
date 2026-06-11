import QtQuick
import QtQuick.Layouts

Rectangle {
    id: contentBlankPageQML
    color: "#f6f8fb"

    ColumnLayout {
        anchors.centerIn: parent
        width: Math.min(parent.width - 48, 420)
        spacing: 12

        Image {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 64
            Layout.preferredHeight: 64
            source: "assets/Icon.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            Layout.fillWidth: true
            text: "Select a message"
            color: "#101828"
            font.pixelSize: 22
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            Layout.fillWidth: true
            text: "Use the Compose button to send a new message through the configured SMTP server."
            color: "#6a7282"
            font.pixelSize: 14
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
