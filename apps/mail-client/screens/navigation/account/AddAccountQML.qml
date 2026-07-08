import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Effects

Rectangle {
    id: rootWindow

    clip: true
    implicitHeight: 460
    implicitWidth: 640
    color: Color.background
    radius: 14
    visible: true

    Rectangle {
        id: backgroundRectangle

        anchors.fill: parent
        border.color: Color.border
        border.width: 1
        color: Color.background
        radius: 14
    }
    Rectangle {
        id: closeButtonRectangle

        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 4
        color: Color.transparent
        height: 40
        radius: 8
        scale: closeClickArea.containsMouse ? 1.3 : 1.0
        width: 40
        z: 10

        Behavior on scale {
            id: closeScaleBehavior

            NumberAnimation {
                id: closeScaleAnimation

                duration: 150
                easing.type: Easing.InOutQuad
            }
        }

        Image {
            id: closeIcon

            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            height: 15
            source: "qrc:/pngs/assets/ic_close_window_black.svg"
            sourceSize.height: height * Screen.devicePixelRatio
            sourceSize.width: width * Screen.devicePixelRatio
            width: 15
        }
        MouseArea {
            id: closeClickArea

            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true

            onClicked: {
                closeAuthWindow();
            }
        }
    }

    // Dynamic screen loader for changing screens
    Loader {
        id: contentLoader

        anchors.fill: parent
        sourceComponent: choiceScreenComponent
    }

    // Event routing from loaded screens
    Connections {
        id: loaderConnections

        // Handle back navigation
        function onBackRequested() {
            contentLoader.sourceComponent = choiceScreenComponent;
        }

        // Handle login submit
        function onLoginSubmitted(email, password) {
            //rootWindow.close()
        }

        // Handle registration submit
        function onRegisterSubmitted(name, email, password) {
            var success = regHandler.registerUser(name, email, password);

            if (success) {
                var firstLetter = avatarInitial(name);

                accountModel.AddAccount(name, email, "", Color.avatar, firstLetter, true);
                CurrentUser.Authorize(name, email, "");

                closeAuthWindow();
            } else {
                return -1;
            }
        }

        ignoreUnknownSignals: true
        target: contentLoader.item
    }
    Component {
        id: choiceScreenComponent

        Item {
            id: choiceScreenRootItem

            anchors.fill: parent

            Column {
                id: mainLayoutColumn

                anchors.centerIn: parent
                spacing: 30

                Text {
                    id: mainTitleText

                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Color.primaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 32
                    font.weight: Font.Bold
                    text: "Add an Account"
                }
                Row {
                    id: optionsRow

                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 24

                    // Login selection
                    Rectangle {
                        id: loginSelectionBlock

                        border.color: Color.outline
                        border.width: 1
                        color: Color.transparent
                        height: 300
                        radius: 12
                        width: 260

                        Column {
                            id: loginContentColumn

                            anchors.centerIn: parent
                            spacing: 16
                            width: parent.width - 32

                            Image {
                                id: loginIcon

                                anchors.horizontalCenter: parent.horizontalCenter
                                fillMode: Image.PreserveAspectFit
                                height: 80
                                source: "qrc:/pngs/assets/ic_login.svg"
                                sourceSize.height: height * Screen.devicePixelRatio
                                sourceSize.width: width * Screen.devicePixelRatio
                                width: 80
                            }
                            Text {
                                id: loginTitleText

                                anchors.horizontalCenter: parent.horizontalCenter
                                color: Color.primaryText
                                font.family: "Segoe UI"
                                font.pixelSize: 22
                                font.weight: Font.Bold
                                text: "Existing User?"
                            }
                            Rectangle {
                                id: loginButton

                                anchors.horizontalCenter: parent.horizontalCenter
                                color: loginClickArea.containsMouse ? Color.buttonSpecialHover : Color.buttonSpecial
                                height: 44
                                radius: 8
                                scale: loginClickArea.containsMouse ? 1.05 : 1.0
                                width: 160

                                Behavior on scale {
                                    id: loginScaleBehavior

                                    NumberAnimation {
                                        id: loginScaleAnimation

                                        duration: 150
                                        easing.type: Easing.InOutQuad
                                    }
                                }

                                Text {
                                    id: loginButtonText

                                    anchors.centerIn: parent
                                    color: Color.buttonSpecialText
                                    font.family: "Segoe UI"
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                    text: "Log In"
                                }
                                MouseArea {
                                    id: loginClickArea

                                    anchors.fill: parent
                                    cursorShape: Qt.PointingHandCursor
                                    hoverEnabled: true

                                    onClicked: {
                                        contentLoader.source = "LoginQML.qml";
                                    }
                                }
                            }
                            Text {
                                id: loginDescriptionText

                                anchors.horizontalCenter: parent.horizontalCenter
                                color: Color.secondaryText
                                font.family: "Segoe UI"
                                font.pixelSize: 14
                                horizontalAlignment: Text.AlignHCenter
                                text: "Access your personal \n or work profile"
                            }
                        }
                    }

                    // Redister selection
                    Rectangle {
                        id: registerSelectionBlock

                        border.color: Color.outline
                        border.width: 1
                        color: Color.transparent
                        height: 300
                        radius: 12
                        width: 260

                        Column {
                            id: registerContentColumn

                            anchors.centerIn: parent
                            anchors.verticalCenterOffset: -2
                            spacing: 16
                            width: parent.width - 32

                            Image {
                                id: registerIcon

                                anchors.horizontalCenter: parent.horizontalCenter
                                fillMode: Image.PreserveAspectFit
                                height: 64
                                source: "qrc:/pngs/assets/ic_add_account.svg"
                                sourceSize.height: height * Screen.devicePixelRatio
                                sourceSize.width: width * Screen.devicePixelRatio
                                width: 64
                            }
                            Text {
                                id: registerTitleText

                                anchors.horizontalCenter: parent.horizontalCenter
                                color: Color.primaryText
                                font.family: "Segoe UI"
                                font.pixelSize: 22
                                font.weight: Font.Bold
                                text: "New User?"
                            }
                            Rectangle {
                                id: registerButton

                                anchors.horizontalCenter: parent.horizontalCenter
                                color: registerClickArea.containsMouse ? Color.buttonSpecialHover : Color.buttonSpecial
                                height: 44
                                radius: 8
                                scale: registerClickArea.containsMouse ? 1.05 : 1.0
                                width: 160

                                Behavior on scale {
                                    id: registerScaleBehavior

                                    NumberAnimation {
                                        id: registerScaleAnimation

                                        duration: 150
                                        easing.type: Easing.InOutQuad
                                    }
                                }

                                Text {
                                    id: registerButtonText

                                    anchors.centerIn: parent
                                    color: Color.buttonSpecialText
                                    font.family: "Segoe UI"
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                    text: "Register"
                                }
                                MouseArea {
                                    id: registerClickArea

                                    anchors.fill: parent
                                    cursorShape: Qt.PointingHandCursor
                                    hoverEnabled: true

                                    onClicked: {
                                        contentLoader.source = "RegistrationQML.qml";
                                    }
                                }
                            }
                            Text {
                                id: registerDescriptionText

                                anchors.horizontalCenter: parent.horizontalCenter
                                color: Color.secondaryText
                                font.family: "Segoe UI"
                                font.pixelSize: 14
                                horizontalAlignment: Text.AlignHCenter
                                text: "Create a new account"
                            }
                        }
                    }
                }
            }
        }
    }
}