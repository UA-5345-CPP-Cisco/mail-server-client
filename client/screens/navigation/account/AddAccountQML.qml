import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Effects

Popup {
    id: root
    width: 640
    height: 460
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay

    // Backround window
    background: Rectangle
    {
        color: "#ffffff"
        radius: 14
        border.color: "#e5e7eb"
        border.width: 1
    }

    // Close button
    Rectangle
    {
        width: 40
        height: 40
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 4
        anchors.rightMargin: 4
        radius: 8
        color: "transparent"
        z: 10
        Image
        {
            anchors.centerIn: parent
            source: "qrc:/pngs/assets/ic_close_window_black.svg"
            width: 15
            height: 15
            sourceSize.width: width * Screen.devicePixelRatio
            sourceSize.height: height * Screen.devicePixelRatio
            fillMode: Image.PreserveAspectFit
        }

        scale: closeMouse.containsMouse ? 1.3 : 1.0

        Behavior on scale
        {
            NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
        }

        MouseArea
        {
            id: closeMouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked:
            {
                root.close()
                contentLoader.sourceComponent = choiceScreen //?????????
            }
        }
    }

    Loader
    {
        id: contentLoader
        anchors.fill: parent
        sourceComponent: choiceScreen
    }


    Connections {
        target: contentLoader.item
        ignoreUnknownSignals: true

        // When "Back" is clicked
        function onBackRequested() {
            contentLoader.sourceComponent = choiceScreen
        }

        // When "Sign In" is clicked
        function onLoginSubmitted(email, password) {
            // authorization logic
            root.close()
        }

        function onRegisterSubmitted(name, email, password) {
            // Register logic
            // root.close()
        }
    }

    Component {
        id: choiceScreen

        Item {
            anchors.fill: parent

            Column
            {
                anchors.centerIn: parent
                spacing: 30

                Text
                {
                    text: "Add an Account"
                    font.family: "Segoe UI"
                    font.pixelSize: 32
                    font.weight: Font.Bold
                    //color: "#6a748b"
                    color: "#000000"
                    anchors.horizontalCenter: parent.horizontalCenter
                }


                Row
                {
                    spacing: 24
                    anchors.horizontalCenter: parent.horizontalCenter

                    // First block
                    Rectangle
                    {
                        width: 260
                        height: 300
                        radius: 12
                        border.color: "#e5e7eb"
                        border.width: 1
                        color: "transparent"
                        Column
                        {
                            anchors.centerIn: parent
                            spacing: 16
                            width: parent.width - 32
                            Image
                            {
                                source: "qrc:/pngs/assets/ic_login.svg"
                                sourceSize.width: 80
                                sourceSize.height: 80
                                fillMode: Image.PreserveAspectFit
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            Text
                            {
                                text: "Existing User?"
                                font.family: "Segoe UI"
                                font.pixelSize: 22
                                font.weight: Font.Bold
                                color: "#6a748b"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            // Log In button
                            Rectangle
                            {
                                width: 160
                                height: 44
                                radius: 8
                                color: loginMouseArea.containsMouse ? "#0052cc" : "#1a66ff"
                                anchors.horizontalCenter: parent.horizontalCenter
                                Text
                                {
                                    anchors.centerIn: parent
                                    text: "Log In"
                                    color: "#ffffff"
                                    font.family: "Segoe UI"
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                }
                                scale: loginMouseArea.containsMouse ? 1.15 : 1.0

                                Behavior on scale
                                {
                                    NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
                                }
                                MouseArea
                                {
                                    id: loginMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked:
                                    {
                                        contentLoader.source = "LoginQML.qml"
                                    }
                                }
                            }

                            Text
                            {
                                text: "Access your personal \n or work profile"
                                font.family: "Segoe UI"
                                font.pixelSize: 14
                                color: "#6a748b"
                                horizontalAlignment: Text.AlignHCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }

                    // Second block
                    Rectangle
                    {
                        width: 260
                        height: 300
                        radius: 12
                        border.color: "#e5e7eb"
                        border.width: 1
                        color: "transparent"
                        Column
                        {
                            anchors.centerIn: parent
                            anchors.verticalCenterOffset: -2
                            spacing: 16
                            width: parent.width - 32
                            Image
                            {
                                source: "qrc:/pngs/assets/ic_add_account.svg"
                                sourceSize.width: 64
                                sourceSize.height: 64
                                fillMode: Image.PreserveAspectFit
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            Text
                            {
                                text: "New User?"
                                font.family: "Segoe UI"
                                font.pixelSize: 22
                                font.weight: Font.Bold
                                color: "#6a748b"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            // Register button
                            Rectangle
                            {
                                width: 160
                                height: 44
                                radius: 8
                                color: registerMouseArea.containsMouse ? "#0052cc" : "#1a66ff"
                                anchors.horizontalCenter: parent.horizontalCenter
                                Text
                                {
                                    anchors.centerIn: parent
                                    text: "Register"
                                    color: "#ffffff"
                                    font.family: "Segoe UI"
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                }
                                scale: registerMouseArea.containsMouse ? 1.15 : 1.0

                                Behavior on scale
                                {
                                    NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
                                }


                                MouseArea
                                {
                                    id: registerMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked:
                                    {
                                        contentLoader.source = "RegisterQML.qml"
                                    }
                                }
                            }

                            Text
                            {
                                text: "Create a new account"
                                font.family: "Segoe UI"
                                font.pixelSize: 14
                                color: "#6a748b"
                                horizontalAlignment: Text.AlignHCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                }
            }
        }

    }
}