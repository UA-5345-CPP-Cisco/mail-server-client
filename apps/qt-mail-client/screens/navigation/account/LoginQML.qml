import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Item {
    id: rootItem
    implicitWidth: 400
    implicitHeight: 350

    signal backRequested()
    signal loginSubmitted(string email, string password)
    property string emailError: ""
    property string passwordError: ""
    property bool passwordVisible: false

    // Button to go back to previous screen
    Rectangle
    {
        id: backButtonRectangle
        width: 40
        height: 40
        anchors.top: parent.top
        anchors.left: parent.left
        radius: 8
        color: "transparent"
        z: 10

        Image
        {
            id: backIconImage
            anchors.centerIn: parent
            source: "qrc:/pngs/assets/ic_button_back.svg"
            width: 15
            height: 15
            sourceSize.width: width * Screen.devicePixelRatio
            sourceSize.height: height * Screen.devicePixelRatio
            fillMode: Image.PreserveAspectFit
        }

        scale: backClickArea.containsMouse ? 1.3 : 1.0

        Behavior on scale
        {
            id: backScaleBehavior

            NumberAnimation
            {
                id: backScaleAnimation
                duration: 150
                easing.type: Easing.InOutQuad
            }
        }

        MouseArea
        {
            id: backClickArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onClicked:
            {
                rootItem.backRequested()
            }
        }
    }

    Column
    {
        id: mainContentColumn
        anchors.centerIn: parent
        spacing: 20
        width: 320

        Text
        {
            id: titleText
            text: "Log In"
            font.family: "Segoe UI"
            font.pixelSize: 28
            font.weight: Font.Black
            color: "#101828"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text
        {
            id: descriptionText
            text: "Please enter your details to sign in"
            font.family: "Segoe UI"
            font.pixelSize: 14
            color: "#6a748b"
            anchors.horizontalCenter: parent.horizontalCenter
            bottomPadding: 10
        }

        // Email field for login screen
        TextField
        {
            id: emailTextField
            width: parent.width
            placeholderText: "Email address"
            font.family: "Segoe UI"
            font.pixelSize: 14
            color: "#101828"
            leftPadding: 16
            rightPadding: 16
            topPadding: 12
            bottomPadding: 12
            cursorDelegate: Item {}

            background: Rectangle
            {
                id: emailBackgroundRectangle
                radius: 8
                border.color: rootItem.emailError !== "" ? "#fda29b" : (emailTextField.activeFocus ? "#1a66ff" : "#e5e7eb")
                border.width: emailTextField.activeFocus || rootItem.emailError !== "" ? 2 : 1
                color: "#ffffff"
            }

            Rectangle
            {
                id: emailCustomCursorRectangle
                width: 1.5
                color: "#1f2937"
                height: parent.font.pixelSize + 4
                anchors.verticalCenter: parent.verticalCenter
                x: parent.length > 0 ? parent.cursorRectangle.x : 14
                visible: parent.activeFocus

                Behavior on x
                {
                    id: emailCursorXBehavior
                    NumberAnimation { id: emailCursorXAnimation; duration: 80; easing.type: Easing.OutCubic }
                }

                SequentialAnimation on opacity
                {
                    id: emailCursorOpacityAnimation
                    running: parent.activeFocus
                    loops: Animation.Infinite
                    NumberAnimation { id: emailCursorFadeOut; to: 0; duration: 400; easing.type: Easing.InOutSine }
                    NumberAnimation { id: emailCursorFadeIn; to: 1; duration: 400; easing.type: Easing.InOutSine }
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
            topPadding: -10
        }


        // Pasword field for login screen
        TextField
        {
            id: passwordTextField
            width: parent.width
            placeholderText: "Password"
            //echoMode: TextInput.Password
            echoMode: passwordVisible ? TextInput.Normal : TextInput.Password
            font.family: "Segoe UI"
            font.pixelSize: 14
            color: "#101828"
            leftPadding: 16
            rightPadding: 16
            topPadding: 12
            bottomPadding: 12
            cursorDelegate: Item {}

            Image
            { 
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

                MouseArea 
                {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: passwordVisible = !passwordVisible
                }
            }

            background: Rectangle
            {
                id: passwordBackgroundRectangle
                radius: 8
                border.color: passwordTextField.activeFocus ? "#1a66ff" : "#e5e7eb"
                border.width: passwordTextField.activeFocus ? 2 : 1
                color: "#ffffff"
            }

            Rectangle
            {
                id: passwordCustomCursorRectangle
                width: 1.5
                color: "#1f2937"
                height: parent.font.pixelSize + 4
                anchors.verticalCenter: parent.verticalCenter
                x: parent.length > 0 ? parent.cursorRectangle.x : 14
                visible: parent.activeFocus

                Behavior on x
                {
                    id: passwordCursorXBehavior
                    NumberAnimation { id: passwordCursorXAnimation; duration: 80; easing.type: Easing.OutCubic }
                }

                SequentialAnimation on opacity
                {
                    id: passwordCursorOpacityAnimation
                    running: parent.activeFocus
                    loops: Animation.Infinite
                    NumberAnimation { id: passwordCursorFadeOut; to: 0; duration: 400; easing.type: Easing.InOutSine }
                    NumberAnimation { id: passwordCursorFadeIn; to: 1; duration: 400; easing.type: Easing.InOutSine }
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
            topPadding: -10
        }

        // Button to login
        Rectangle
        {
            id: loginButtonRectangle
            width: parent.width
            height: 44
            radius: 8
            color: loginClickArea.containsMouse ? "#0052cc" : "#1a66ff"

            scale: loginClickArea.containsMouse ? 1.03 : 1.0

            Behavior on scale
            {
                id: loginScaleBehavior

                NumberAnimation
                {
                    id: loginScaleAnimation
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            Text
            {
                id: loginButtonText
                anchors.centerIn: parent
                text: "Sign In"
                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 16
                font.weight: Font.Bold
            }

            MouseArea
            {
                id: loginClickArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked:
                {
                    var emailErr = rootWindow.getValidationError("email", emailTextField.text)
                    var pwdErr = rootWindow.getValidationError("password", passwordTextField.text)
                    emailError = emailErr
                    passwordError = pwdErr
                    //rootItem.loginSubmitted(emailTextField.text, passwordTextField.text)
                }
            }
        }
    }
}