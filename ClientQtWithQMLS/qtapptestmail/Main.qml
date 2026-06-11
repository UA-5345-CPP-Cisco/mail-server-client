import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

ApplicationWindow {
    id: window
    visible: true
    width: 1024
    height: 768
    minimumHeight: 500
    minimumWidth: 750
    title: "Mail Client Interface"

    MouseArea {
        anchors.fill: parent
        onClicked: {
            window.contentItem.forceActiveFocus()
        }
    }

    function closeMessageWindow() {
        newMessageLoader.active = false
        newMessageLoader.source = ""
    }

    function closeSettingsWindow() {
        settingsLoader.active = false
        settingsLoader.source = ""
    }

    Item {
        id: mainContent
        anchors.fill: parent

        SplitView {
            id: splitView
            anchors.fill: parent
            orientation: Qt.Horizontal

            handle: Rectangle {
                id: handleDelegate
                color: "transparent"
                containmentMask: Item
                {
                    x: (handleDelegate.width - width) / 2
                    width: 5
                    height: splitView.height
                    MouseArea
                    {
                        anchors.fill: parent

                        hoverEnabled: true

                        cursorShape: Qt.SizeHorCursor

                        acceptedButtons: Qt.NoButton
                    }
                }
            }


            NavigationQML {
                id: navMenu
                SplitView.preferredWidth: 250
                SplitView.minimumWidth: 180
                SplitView.maximumWidth: 350
                SplitView.fillHeight: true
            }

            EmailsListQML {
                SplitView.preferredWidth: 350
                SplitView.minimumWidth: 250
                SplitView.fillHeight: true
            }

            ContentBlankPageQML {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                SplitView.minimumWidth: 250
            }
        }
    }

    MultiEffect {
        id: blurEffect
        anchors.fill: mainContent
        source: mainContent

        blur: settingsLoader.opacity * 0.5
        blurEnabled: settingsLoader.active
        autoPaddingEnabled: false

        Behavior on blur {
            NumberAnimation { duration: 200 }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#000000"
        opacity: settingsLoader.opacity * 0.4
        visible: settingsLoader.active
        z:998
        MouseArea
        {

            hoverEnabled: true
            preventStealing: true
            enabled: parent.visible

            anchors.fill: parent
            onPressed: (mouse) => mouse.accepted = true
            onReleased: (mouse) => mouse.accepted = true
            onClicked: (mouse) => mouse.accepted = true
            onDoubleClicked: (mouse) => mouse.accepted = true
            onPositionChanged: (mouse) => mouse.accepted = true


            onWheel: (wheel) => {
                wheel.accepted = true
            }
        }
    }

    Loader {
        id: newMessageLoader
        active: false
        z: 999
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        width: item ? item.implicitWidth : 0
        height: item ? item.implicitHeight : 0
        source: ""

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
        opacity: status === Loader.Ready ? 1 : 0
    }

    Loader {
        id: settingsLoader
        active: false
        z: 1000

        anchors.centerIn: parent

        width: item ? item.implicitWidth : 0
        height: item ? item.implicitHeight : 0

        Shortcut {
            sequence: "Escape"
            enabled: settingsLoader.active
            onActivated: {
                closeSettingsWindow()
            }
        }

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
        opacity: status === Loader.Ready ? 1 : 0
    }
}