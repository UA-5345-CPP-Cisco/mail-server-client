import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    visible: true
    width: 1024
    height: 768
    minimumWidth: 750
    minimumHeight: 500
    title: "Mail Client"
    color: "#f6f8fb"

    function closeMessageWindow() {
        newMessageLoader.active = false
        newMessageLoader.source = ""
    }

    Loader {
        id: newMessageLoader
        active: false
        z: 999
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 18
        anchors.bottomMargin: 18
        source: active ? "NewMessageQML.qml" : ""
    }

    Popup {
        id: settingsPopup
        modal: true
        focus: true
        padding: 0
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        anchors.centerIn: Overlay.overlay
        background: Rectangle {
            color: "transparent"
        }

        SettingsQML {
            onCloseRequested: settingsPopup.close()
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        NavigationQML {
            Layout.preferredWidth: 250
            Layout.fillHeight: true
            onComposeRequested: {
                newMessageLoader.active = true
            }
            onSettingsRequested: settingsPopup.open()
        }

        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: Qt.Horizontal

            handle: Rectangle {
                implicitWidth: 1
                color: SplitHandle.hovered ? "#cbd5e1" : "#e5e7eb"
            }

            EmailsListQML {
                SplitView.preferredWidth: 350
                SplitView.minimumWidth: 260
                SplitView.fillHeight: true
            }

            ContentBlankPageQML {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                SplitView.minimumWidth: 280
            }
        }
    }
}
