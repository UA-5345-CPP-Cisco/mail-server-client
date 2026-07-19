import QtQuick

Rectangle {
    id: statePopup

    property string message: ""

    // how far below the resting position the popup starts/ends, i.e. how deep "into" the footer it sits when hidden
    property real footerOffset: 8

    signal cancelClicked()
    signal closeRequested()

    // shows the popup by playing the entrance animation (can be called again and again)
    function show() {
        exitAnimation.stop();
        visible = true;
        entranceAnimation.restart();
        hideTimer.repeatCycles = 4;
        hideTimer.restart();
    }

    // plays the exit animation, then hides and notifies listeners
    function hide() {
        hideTimer.stop();
        hideTimer.repeatCycles = 0;
        if (!visible) return;
        entranceAnimation.stop();
        exitAnimation.restart();
    }

    implicitWidth: 260
    implicitHeight: 44
    radius: 10
    color: "#1e2939"
    border.color: "#324156"
    border.width: 1
    visible: false

    // resting values used by the animations below
    opacity: 0
    scale: 0.9

    // translated independently of anchors, so it slides relative to wherever the popup is anchored (e.g. above the footer)
    transform: Translate {
        id: popupTranslate
        y: statePopup.footerOffset
    }

    ParallelAnimation {
        id: entranceAnimation

        NumberAnimation {
            target: statePopup
            property: "opacity"
            to: 1
            duration: 220
            easing.type: Easing.OutCubic
        }
        NumberAnimation {
            target: statePopup
            property: "scale"
            to: 1.0
            duration: 220
            easing.type: Easing.OutBack
            easing.overshoot: 1.4
        }
        NumberAnimation {
            target: popupTranslate
            property: "y"
            to: 0
            duration: 220
            easing.type: Easing.OutCubic
        }
    }

    SequentialAnimation {
        id: exitAnimation

        ParallelAnimation {
            NumberAnimation {
                target: statePopup
                property: "opacity"
                to: 0
                duration: 180
                easing.type: Easing.InCubic
            }
            NumberAnimation {
                target: statePopup
                property: "scale"
                to: 0.9
                duration: 180
                easing.type: Easing.InCubic
            }
            NumberAnimation {
                target: popupTranslate
                property: "y"
                to: statePopup.footerOffset
                duration: 180
                easing.type: Easing.InCubic
            }
        }
        ScriptAction {
            script: {
                statePopup.visible = false;
                statePopup.closeRequested();
            }
        }
    }

    Timer {
        id: hideTimer
        interval: 1000
        repeat: true
        running: true
        property int repeatCycles: 4

        onTriggered: {
            repeatCycles--

            if (repeatCycles <= 0) {
                stop()
                hide()
            }
        }
    }

    // message text
    Text {
        id: messageText
        height: 16

        anchors.left: parent.left
        anchors.leftMargin: 47
        anchors.right: cancelWrapper.left
        anchors.rightMargin: 8
        anchors.verticalCenter: parent.verticalCenter

        color: "#ffffff"
        elide: Text.ElideRight
        font.family: "Segoe UI"
        font.pixelSize: 12
        text: message
        verticalAlignment: Text.AlignVCenter
    }

    // cancel button
    Text {
        id: timeText
        text: hideTimer.repeatCycles + "s"
        anchors.left: parent.left
        anchors.right: messageText.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 20
        anchors.rightMargin: -29
        anchors.topMargin: 14
        anchors.bottomMargin: 14
        font.pixelSize: 12
        color: "#ffffff"
    }

    Rectangle {
        id: cancelWrapper

        anchors.right: closeWrapper.left
        anchors.rightMargin: 4
        anchors.verticalCenter: parent.verticalCenter
        color: "transparent"
        height: 22
        width: cancelText.implicitWidth + 12

        Text {
            id: cancelText

            anchors.centerIn: parent
            color: "#46b2c6"
            font.family: "Segoe UI"
            font.pixelSize: 12
            opacity: cancelClickArea.containsMouse ? 1.0 : 0.85
            text: qsTr("Cancel")

            Behavior on opacity {
                NumberAnimation {
                    duration: 120
                    easing.type: Easing.InOutQuad
                }
            }
        }

        scale: cancelClickArea.containsMouse ? 1.06 : 1.0

        Behavior on scale {
            NumberAnimation {
                duration: 150
                easing.type: Easing.InOutQuad
            }
        }

        MouseArea {
            id: cancelClickArea

            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true

            onClicked: {
                statePopup.cancelClicked();
                statePopup.hide();
            }
        }
    }

    // close button
    Rectangle {
        id: closeWrapper

        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        color: closeClickArea.pressed ? "#ffdede" : "transparent"
        height: 22
        radius: 4
        width: 22

        Behavior on color {
            ColorAnimation {
                duration: 120
            }
        }

        Image {
            id: closeIcon

            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            height: 12
            scale: closeClickArea.containsMouse ? 1.3 : 1.0
            source: "qrc:/pngs/assets/ic_close_window.svg"
            sourceSize.height: 96
            sourceSize.width: 96
            width: 12

            Behavior on scale {
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }
        }

        MouseArea {
            id: closeClickArea

            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true

            onClicked: {
                statePopup.hide();
            }
        }
    }

}
