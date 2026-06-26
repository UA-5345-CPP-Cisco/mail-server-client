import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Item {
    id: rootItem
    implicitWidth: 400
    implicitHeight: 350

    signal backRequested()
    signal registerSubmitted(string name, string email, string password)

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
            text: "Register"
            font.family: "Segoe UI"
            font.pixelSize: 28
            font.weight: Font.Bold
            color: "#101828"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text
        {
            id: descriptionText
            text: "Please enter your details to create a new account"
            font.family: "Segoe UI"
            font.pixelSize: 14
            color: "#6a748b"
            anchors.horizontalCenter: parent.horizontalCenter
            bottomPadding: 10
        }

        // Name field for register screen
        TextField
        {
            id: fullNameTextField
            width: parent.width
            placeholderText: "Full name"
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
                id: fullNameBackgroundRectangle
                radius: 8
                border.color: fullNameTextField.activeFocus ? "#1a66ff" : "#e5e7eb"
                border.width: fullNameTextField.activeFocus ? 2 : 1
                color: "#ffffff"
            }

            Rectangle
            {
                id: nameCustomCursorRectangle
                width: 1.5
                color: "#1f2937"
                height: parent.font.pixelSize + 4
                anchors.verticalCenter: parent.verticalCenter
                x: parent.length > 0 ? parent.cursorRectangle.x : 14
                visible: parent.activeFocus

                Behavior on x
                {
                    id: nameCursorXBehavior
                    NumberAnimation { id: nameCursorXAnimation; duration: 80; easing.type: Easing.OutCubic }
                }

                SequentialAnimation on opacity
                {
                    id: nameCursorOpacityAnimation
                    running: parent.activeFocus
                    loops: Animation.Infinite
                    NumberAnimation { id: nameCursorFadeOut; to: 0; duration: 400; easing.type: Easing.InOutSine }
                    NumberAnimation { id: nameCursorFadeIn; to: 1; duration: 400; easing.type: Easing.InOutSine }
                }
            }
        }

        // Email field for register screen
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
                border.color: emailTextField.activeFocus ? "#1a66ff" : "#e5e7eb"
                border.width: emailTextField.activeFocus ? 2 : 1
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
        }

        // Pasword field for register screen
        TextField
        {
            id: passwordTextField
            width: parent.width
            placeholderText: "Password"
            echoMode: TextInput.Password
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
        }

        // Basic register
        Rectangle
        {
            id: registerButtonRectangle
            width: parent.width
            height: 44
            radius: 8
            color: registerClickArea.containsMouse ? "#0052cc" : "#1a66ff"

            scale: registerClickArea.containsMouse ? 1.03 : 1.0

            Behavior on scale
            {
                id: registerScaleBehavior
                NumberAnimation { id: registerScaleAnimation; duration: 150; easing.type: Easing.InOutQuad }
            }

            Behavior on color
            {
                id: registerColorBehavior
                ColorAnimation { id: registerColorAnimation; duration: 150 }
            }

            Text
            {
                id: registerButtonText
                anchors.centerIn: parent
                text: "Register"
                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 16
                font.weight: Font.Bold
            }

            MouseArea
            {
                id: registerClickArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked:
                {
                    rootItem.registerSubmitted(fullNameTextField.text, emailTextField.text, passwordTextField.text)
                }
            }
        }

        // Button for registration through Google
        Rectangle
        {
            id: googleRegisterButtonRectangle
            width: parent.width
            height: 44
            radius: 8
            color: googleClickArea.containsMouse ? "#f5f5f5" : "#ffffff"
            border.color: "#e5e7eb"
            border.width: 1

            scale: googleClickArea.containsMouse ? 1.03 : 1.0

            Behavior on scale
            {
                id: googleScaleBehavior
                NumberAnimation { id: googleScaleAnimation; duration: 150; easing.type: Easing.InOutQuad }
            }

            Behavior on color
            {
                id: googleColorBehavior
                ColorAnimation { id: googleColorAnimation; duration: 150 }
            }

            Row
            {
                id: googleContentRow
                anchors.centerIn: parent
                spacing: 10

                Image
                {
                    id: googleIconImage
                    width: 20
                    height: 20
                    source: "qrc:/pngs/assets/ic_google_logo.svg"
                    sourceSize.width: width * Screen.devicePixelRatio
                    sourceSize.height: height * Screen.devicePixelRatio
                    fillMode: Image.PreserveAspectFit
                }

                Text
                {
                    id: googleButtonText
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Register with Google"
                    color: "#374151"
                    font.family: "Segoe UI"
                    font.pixelSize: 16
                    font.weight: Font.Medium
                }
            }

            MouseArea
            {
                id: googleClickArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked:
                {
                    // method
                }
            }
        }
    }
}