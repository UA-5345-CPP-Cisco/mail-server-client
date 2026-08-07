import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Item {
    id: rootItem

    signal backRequested
    signal registerSubmitted(string name, string email, string password)
    property string nameError: ""
    property string emailError: ""
    property string passwordError: ""
    property string confirmPasswordError: ""
    property bool passwordVisible: false
    property string generalError: ""
    property bool confirmPasswordVisible: false

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
            width: 15
            source: "qrc:/pngs/assets/ic_button_back.svg"
            sourceSize.height: height * Screen.devicePixelRatio
            sourceSize.width: width * Screen.devicePixelRatio
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
        spacing: 15
        width: 320

        Text {
            id: titleText

            anchors.horizontalCenter: parent.horizontalCenter
            color: Color.primaryText
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
            color: Color.primaryText
            font.family: "Segoe UI"
            font.pixelSize: 14
            leftPadding: 16
            placeholderText: "Name"
            placeholderTextColor: Color.secondaryText
            rightPadding: 16
            topPadding: 12
            width: parent.width

            background: Rectangle {
                id: fullNameBackgroundRectangle
                border.color: rootItem.nameError !== "" ? "#fda29b" : (fullNameTextField.activeFocus ? "#1a66ff" : "#e5e7eb")
                border.width: fullNameTextField.activeFocus ? 2 : 1
                color: Color.background
                radius: 8
            }
            cursorDelegate: Item {
            }

            Rectangle {
                id: nameCustomCursorRectangle

                anchors.verticalCenter: parent.verticalCenter
                color: Color.secondaryText
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
            onTextChanged: rootItem.nameError = ""
        }

        // Error message for name field
        Text {
            text: rootItem.nameError
            color: "#f04438"
            font.family: "Segoe UI"
            font.pixelSize: 12
            visible: nameError !== ""
            topPadding: -14
        }

        // Email field for register screen
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
                border.color: rootItem.emailError !== "" ? "#fda29b" : (emailTextField.activeFocus ? "#1a66ff" : "#e5e7eb")
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
            onTextChanged: rootItem.emailError = ""
        }

        // Error message for email field
        Text {
            text: rootItem.emailError
            color: "#f04438"
            font.family: "Segoe UI"
            font.pixelSize: 12
            visible: emailError !== ""
            topPadding: -14
        }

        // Password field for register screen
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
                
                border.color: rootItem.passwordError !== "" ? "#fda29b" : (passwordTextField.activeFocus ? "#1a66ff" : "#e5e7eb")
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

                    NumberAnimation {
                        id: passwordCursorXAnimation

                        duration: 80
                        easing.type: Easing.OutCubic
                    }
                }
            }
            onTextChanged: rootItem.passwordError = ""
        }

        // Error message for password field
        Text {
            text: rootItem.passwordError
            color: "#f04438"
            font.family: "Segoe UI"
            font.pixelSize: 12
            visible: passwordError !== ""
            topPadding: -14
        }

        // Password field for password confirmation
        TextField {
            id: passwordConfirmationTextField

            bottomPadding: 12
            color: Color.primaryText
            echoMode: confirmPasswordVisible ? TextInput.Normal : TextInput.Password
            font.family: "Segoe UI"
            font.pixelSize: 14
            leftPadding: 16
            placeholderText: "Confirm Password"
            placeholderTextColor: Color.secondaryText
            rightPadding: 16
            topPadding: 12
            width: parent.width

            Image {
                id: confirmEyeIcon
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 12
                source: confirmPasswordVisible ? "qrc:/pngs/assets/ic_eye_open.svg" : "qrc:/pngs/assets/ic_eye_closed.svg"
                width: 20
                height: 20
                sourceSize.width: width * Screen.devicePixelRatio
                sourceSize.height: height * Screen.devicePixelRatio
                fillMode: Image.PreserveAspectFit

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: confirmPasswordVisible = !confirmPasswordVisible
                }
            }

            background: Rectangle {
                id: confirmBackgroundRectangle
                
                border.color: rootItem.confirmPasswordError !== "" ? "#fda29b" : (passwordConfirmationTextField.activeFocus ? "#1a66ff" : "#e5e7eb")
                border.width: passwordConfirmationTextField.activeFocus ? 2 : 1
                color: Color.background
                radius: 8
            }
            cursorDelegate: Item {}

            Rectangle {
                id: confirmCustomCursorRectangle

                anchors.verticalCenter: parent.verticalCenter
                color: Color.secondaryText
                height: parent.font.pixelSize + 4
                visible: parent.activeFocus
                width: 1.5
                x: parent.length > 0 ? parent.cursorRectangle.x : 14

                SequentialAnimation on opacity {
                    id: confirmCursorOpacityAnimation

                    loops: Animation.Infinite
                    running: parent.activeFocus

                    NumberAnimation {
                        id: confirmCursorFadeOut

                        duration: 400
                        easing.type: Easing.InOutSine
                        to: 0
                    }
                    NumberAnimation {
                        id: confirmCursorFadeIn

                        duration: 400
                        easing.type: Easing.InOutSine
                        to: 1
                    }
                }
                Behavior on x {
                    id: confirmCursorXBehavior

                    NumberAnimation {
                        id: confirmCursorXAnimation

                        duration: 80
                        easing.type: Easing.OutCubic
                    }
                }
            }
            onTextChanged: rootItem.confirmPasswordError = ""
        }

        // Error message for password confirmation field
        Text {
            text: rootItem.confirmPasswordError
            color: "#f04438"
            font.family: "Segoe UI"
            font.pixelSize: 12
            visible: confirmPasswordError !== ""
            topPadding: -14
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
                    rootItem.generalError = ""
                    var name_error = rootWindow.getRegisterValidationError("name", fullNameTextField.text)
                    var email_error = rootWindow.getRegisterValidationError("email", emailTextField.text)
                    var password_error = rootWindow.getRegisterValidationError("password", passwordTextField.text)
                    var confirm_password_error = ""
                    if (passwordConfirmationTextField.text === "") 
                    {
                        confirm_password_error = "Please confirm your password"
                    } 
                    else if (passwordTextField.text !== passwordConfirmationTextField.text) 
                    {
                        confirm_password_error = "Passwords do not match"
                    }

                    nameError = name_error
                    emailError = email_error
                    passwordError = password_error
                    confirmPasswordError = confirm_password_error
                    if(nameError === "" && emailError === "" && passwordError === "" && confirmPasswordError === "")
                    {
                        rootItem.registerSubmitted(fullNameTextField.text, emailTextField.text, passwordTextField.text)
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
        }
    }
}