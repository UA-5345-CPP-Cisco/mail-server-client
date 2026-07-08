import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Item {
    id: rootItem

    signal backRequested
    signal registerSubmitted(string name, string email, string password)

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
            color: Color.button
            font.family: "Segoe UI"
            font.pixelSize: 28
            font.weight: Font.Bold
            text: "Register"
        }
        Text {
            id: descriptionText

            anchors.horizontalCenter: parent.horizontalCenter
            bottomPadding: 10
            color: Color.secondaryText
            font.family: "Segoe UI"
            font.pixelSize: 14
            text: "Please enter your details to create a new account"
        }

        // Name field for register screen
        TextField {
            id: fullNameTextField

            bottomPadding: 12
            color: Color.button
            font.family: "Segoe UI"
            font.pixelSize: 14
            leftPadding: 16
            placeholderText: "Full name"
            rightPadding: 16
            topPadding: 12
            width: parent.width

            background: Rectangle {
                id: fullNameBackgroundRectangle

                border.color: fullNameTextField.activeFocus ? Color.hover : Color.outline
                border.width: fullNameTextField.activeFocus ? 2 : 1
                color: Color.background
                radius: 8
            }
            cursorDelegate: Item {
            }

            Rectangle {
                id: nameCustomCursorRectangle

                anchors.verticalCenter: parent.verticalCenter
                color: Color.hover
                height: parent.font.pixelSize + 4
                visible: parent.activeFocus
                width: 1.5
                x: parent.length > 0 ? parent.cursorRectangle.x : 14

                SequentialAnimation on opacity {
                    id: nameCursorOpacityAnimation

                    loops: Animation.Infinite
                    running: parent.activeFocus

                    NumberAnimation {
                        id: nameCursorFadeOut

                        duration: 400
                        easing.type: Easing.InOutSine
                        to: 0
                    }
                    NumberAnimation {
                        id: nameCursorFadeIn

                        duration: 400
                        easing.type: Easing.InOutSine
                        to: 1
                    }
                }
                Behavior on x {
                    id: nameCursorXBehavior

                    NumberAnimation {
                        id: nameCursorXAnimation

                        duration: 80
                        easing.type: Easing.OutCubic
                    }
                }
            }
        }

        // Email field for register screen
        TextField {
            id: emailTextField

            bottomPadding: 12
            color: Color.button
            font.family: "Segoe UI"
            font.pixelSize: 14
            leftPadding: 16
            placeholderText: "Email address"
            rightPadding: 16
            topPadding: 12
            width: parent.width

            background: Rectangle {
                id: emailBackgroundRectangle

                border.color: emailTextField.activeFocus ? Color.hover : Color.outline
                border.width: emailTextField.activeFocus ? 2 : 1
                color: Color.background
                radius: 8
            }
            cursorDelegate: Item {
            }

            Rectangle {
                id: emailCustomCursorRectangle

                anchors.verticalCenter: parent.verticalCenter
                color: Color.hover
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

        // Pasword field for register screen
        TextField {
            id: passwordTextField

            bottomPadding: 12
            color: Color.button
            echoMode: TextInput.Password
            font.family: "Segoe UI"
            font.pixelSize: 14
            leftPadding: 16
            placeholderText: "Password"
            rightPadding: 16
            topPadding: 12
            width: parent.width

            background: Rectangle {
                id: passwordBackgroundRectangle

                border.color: passwordTextField.activeFocus ? Color.hover : Color.outline
                border.width: passwordTextField.activeFocus ? 2 : 1
                color: Color.background
                radius: 8
            }
            cursorDelegate: Item {
            }

            Rectangle {
                id: passwordCustomCursorRectangle

                anchors.verticalCenter: parent.verticalCenter
                color: Color.hover
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

                    NumberAnimation {
                        id: passwordCursorXAnimation

                        duration: 80
                        easing.type: Easing.OutCubic
                    }
                }
            }
        }

        // Basic register
        Rectangle {
            id: registerButtonRectangle

            color: registerClickArea.containsMouse ? Color.buttonSpecialHover : Color.buttonSpecial
            height: 44
            radius: 8
            scale: registerClickArea.containsMouse ? 1.03 : 1.0
            width: parent.width

            Behavior on color {
                id: registerColorBehavior

                ColorAnimation {
                    id: registerColorAnimation

                    duration: 150
                }
            }
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
                color: Color.background
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
                    rootItem.registerSubmitted(fullNameTextField.text, emailTextField.text, passwordTextField.text);
                }
            }
        }

        // Button for registration through Google
        Rectangle {
            id: googleRegisterButtonRectangle

            border.color: Color.outline
            border.width: 1
            color: googleClickArea.containsMouse ? Color.hover : Color.button
            height: 44
            radius: 8
            scale: googleClickArea.containsMouse ? 1.03 : 1.0
            width: parent.width

            Behavior on color {
                id: googleColorBehavior

                ColorAnimation {
                    id: googleColorAnimation

                    duration: 150
                }
            }
            Behavior on scale {
                id: googleScaleBehavior

                NumberAnimation {
                    id: googleScaleAnimation

                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            Row {
                id: googleContentRow

                anchors.centerIn: parent
                spacing: 10

                Image {
                    id: googleIconImage

                    fillMode: Image.PreserveAspectFit
                    height: 20
                    source: "qrc:/pngs/assets/ic_google_logo.svg"
                    sourceSize.height: height * Screen.devicePixelRatio
                    sourceSize.width: width * Screen.devicePixelRatio
                    width: 20
                }
                Text {
                    id: googleButtonText

                    anchors.verticalCenter: parent.verticalCenter
                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 16
                    font.weight: Font.Medium
                    text: "Register with Google"
                }
            }
            MouseArea {
                id: googleClickArea

                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: {
                    // method
                }
            }
        }
    }
}