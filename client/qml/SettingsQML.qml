import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: settingsQML
    implicitWidth: 430
    implicitHeight: 460
    color: "#ffffff"
    radius: 14
    border.color: "#e5e7eb"
    clip: true

    signal closeRequested()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true

            Text {
                Layout.fillWidth: true
                text: "Settings"
                color: "#101828"
                font.pixelSize: 18
                font.bold: true
            }

            Button {
                text: "x"
                flat: true
                onClicked: settingsQML.closeRequested()
            }
        }

        GridLayout {
            Layout.fillWidth: true
            columns: 2
            columnSpacing: 10
            rowSpacing: 8

            Label { text: "Server" }
            TextField {
                Layout.fillWidth: true
                text: mailClient.serverHost
                onEditingFinished: mailClient.serverHost = text
            }

            Label { text: "Port" }
            SpinBox {
                Layout.fillWidth: true
                from: 1
                to: 65535
                value: mailClient.serverPort
                editable: true
                onValueModified: mailClient.serverPort = value
            }

            Label { text: "Client name" }
            TextField {
                Layout.fillWidth: true
                text: mailClient.clientName
                onEditingFinished: mailClient.clientName = text
            }

            Label { text: "TLS" }
            ComboBox {
                Layout.fillWidth: true
                model: ["Off", "Auto", "Required"]
                currentIndex: mailClient.tlsMode
                onActivated: mailClient.tlsMode = currentIndex
            }

            Label { text: "From" }
            TextField {
                Layout.fillWidth: true
                text: mailClient.senderAddress
                onEditingFinished: mailClient.senderAddress = text
            }

            Label { text: "Name" }
            TextField {
                Layout.fillWidth: true
                text: mailClient.senderName
                onEditingFinished: mailClient.senderName = text
            }

            Label { text: "Username" }
            TextField {
                Layout.fillWidth: true
                text: mailClient.username
                placeholderText: "Optional"
                onEditingFinished: mailClient.username = text
            }

            Label { text: "Password" }
            TextField {
                Layout.fillWidth: true
                text: mailClient.password
                echoMode: TextInput.Password
                placeholderText: "Optional"
                onEditingFinished: mailClient.password = text
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Button {
                text: mailClient.busy ? "Checking..." : "Check server"
                enabled: !mailClient.busy
                onClicked: mailClient.checkConnection()
            }

            Text {
                Layout.fillWidth: true
                text: mailClient.statusMessage
                color: "#6a7282"
                font.pixelSize: 12
                elide: Text.ElideRight
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Mail App v1.0"
            color: "#6a7282"
            font.pixelSize: 12
        }
    }
}
