import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Item {
    id: rootItem

    signal backRequested
    signal loginSubmitted(string email, string password)
    property string generalError: ""
    property bool passwordVisible: false

    implicitHeight: 350
    implicitWidth: 400

    // Button to go back to previous screen
    Rectangle {
        id: backButtonRectangle

        anchors.left: parent.left
        anchors.top: parent.top
        color: Color.transparent
        height: 40
        radius: 8
        scale: backClickArea.containsMouse ? 1.3 : 1.0
        width: 40
        z: 10

        Behavior on scale {
            id: backScaleBehavior

            NumberAnimation {
                id: backScaleAnimation

                duration: 150
                easing.type: Easing.InOutQuad
            }
        }

        Image {
            id: backIconImage

            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            height: 15
            source: "qrc:/pngs/assets/ic_button_back.svg"
            sourceSize.height: height * Screen.devicePixelRatio
            sourceSize.width: width * Screen.devicePixelRatio
            width: 15
        }
        MouseArea {
            id: backClickArea

            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true

            onClicked: {
                rootItem.backRequested();
            }
        }
    }
    Column {
        id: mainContentColumn

        anchors.centerIn: parent
        spacing: 20
        width: 320

        Text {
            id: titleText

            anchors.horizontalCenter: parent.horizontalCenter
            color: Color.primaryText
            font.family: "Segoe UI"
            font.pixelSize: 28
            font.weight: Font.Black
            text: "Log In"
        }
        Text {
            id: descriptionText

            anchors.horizontalCenter: parent.horizontalCenter
            bottomPadding: 10
            color: Color.secondaryText
            font.family: "Segoe UI"
            font.pixelSize: 14
            text: "Please enter your details to sign in"
        }

        // Email field for login screen
        TextField {
            id: emailTextField

            bottomPadding: 12
            color: Color.primaryText
            font.family: "Segoe UI"
            font.pixelSize: 14
            leftPadding: 16
            placeholderText: "Email address"
            placeholderTextColor: Color.secondaryText
            rightPadding: 16
            topPadding: 12
            width: parent.width

            background: Rectangle {
                id: emailBackgroundRectangle

                border.color: emailTextField.activeFocus ? "#1a66ff" : "#e5e7eb"
                border.width: emailTextField.activeFocus ? 2 : 1
                color: Color.background
                radius: 8
            }
            cursorDelegate: Item {
            }

            Rectangle {
                id: emailCustomCursorRectangle

                anchors.verticalCenter: parent.verticalCenter
                color: Color.secondaryText
                height: parent.font.pixelSize + 4
                visible: parent.activeFocus
                width: 1.5
                x: parent.length > 0 ? parent.cursorRectangle.x : 14

                SequentialAnimation on opacity {
                    id: emailCursorOpacityAnimation

                    loops: Animation.Infinite
                    running: parent.activeFocus

                    NumberAnimation {
                        id: emailCursorFadeOut

                        duration: 400
                        easing.type: Easing.InOutSine
                        to: 0
                    }
                    NumberAnimation {
                        id: emailCursorFadeIn

                        duration: 400
                        easing.type: Easing.InOutSine
                        to: 1
                    }
                }
                Behavior on x {
                    id: emailCursorXBehavior

                    NumberAnimation {
                        id: emailCursorXAnimation

                        duration: 80
                        easing.type: Easing.OutCubic
                    }
                }
            }
        }

        // Password field for login screen
        TextField {
            id: passwordTextField

            bottomPadding: 12
            color: Color.primaryText
            echoMode: passwordVisible ? TextInput.Normal : TextInput.Password
            font.family: "Segoe UI"
            font.pixelSize: 14
            leftPadding: 16
            placeholderText: "Password"
            placeholderTextColor: Color.secondaryText
            rightPadding: 16
            topPadding: 12
            width: parent.width

            Image { 
                id: eyeIcon
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 12
                source: passwordVisible ? "qrc:/pngs/assets/ic_eye_open.svg" : "qrc:/pngs/assets/ic_eye_closed.svg"
                width: 20
                height: 20
                sourceSize.width: width * Screen.devicePixelRatio
                sourceSize.height: height * Screen.devicePixelRatio
                fillMode: Image.PreserveAspectFit

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: passwordVisible = !passwordVisible
                }
            }

            background: Rectangle {
                id: passwordBackgroundRectangle
                border.color: passwordTextField.activeFocus ? "#1a66ff" : "#e5e7eb"
                border.width: passwordTextField.activeFocus ? 2 : 1
                color: Color.background
                radius: 8
            }
            cursorDelegate: Item {}

            Rectangle {
                id: passwordCustomCursorRectangle

                anchors.verticalCenter: parent.verticalCenter
                color: Color.secondaryText
                height: parent.font.pixelSize + 4
                visible: parent.activeFocus
                width: 1.5
                x: parent.length > 0 ? parent.cursorRectangle.x : 14

                SequentialAnimation on opacity {
                    id: passwordCursorOpacityAnimation

                    loops: Animation.Infinite
                    running: parent.activeFocus

                    NumberAnimation {
                        id: passwordCursorFadeOut

                        duration: 400
                        easing.type: Easing.InOutSine
                        to: 0
                    }
                    NumberAnimation {
                        id: passwordCursorFadeIn

                        duration: 400
                        easing.type: Easing.InOutSine
                        to: 1
                    }
                }
                Behavior on x {
                    id: passwordCursorXBehavior

                    NumberAnimation 
                    {
                        id: passwordCursorXAnimation

                        duration: 80
                        easing.type: Easing.OutCubic
                    }
                }
            }
        }

        // Button to login
        Rectangle {
            id: loginButtonRectangle

            color: loginClickArea.containsMouse ? Color.buttonSpecialHover : Color.buttonSpecial
            height: 44
            radius: 8
            scale: loginClickArea.containsMouse ? 1.03 : 1.0
            width: parent.width

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
                text: "Sign In"
            }
            MouseArea {
                id: loginClickArea

                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: {
                    rootItem.generalError = "" 
    
                    if (emailTextField.text === "" || passwordTextField.text === "") 
                    {
                        rootItem.generalError = "Please fill in all fields"
                    }
                    else 
                    {
                        rootItem.loginSubmitted(emailTextField.text, passwordTextField.text);
                    }
                }
            }
        }

        Text {
            id: generalErrorText
            text: rootItem.generalError
            color: "#f04438"
            font.family: "Segoe UI"
            font.pixelSize: 12
            visible: rootItem.generalError !== ""
            anchors.horizontalCenter: parent.horizontalCenter
            topPadding: -10
        }
    }
}