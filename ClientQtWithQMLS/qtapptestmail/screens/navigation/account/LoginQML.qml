import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Item {
    id: root
    implicitWidth: 400
    implicitHeight: 350

    signal backRequested()
    signal loginSubmitted(string email, string password)

    // Back button
    Rectangle
    {
        width: 40
        height: 40
        anchors.top: parent.top
        anchors.left: parent.left
        radius: 8
        color: "transparent"
        z: 10

        Image
        {
            anchors.centerIn: parent
            source: "qrc:/pngs/assets/ic_button_back.svg"
            width: 15
            height: 15
            sourceSize.width: width * Screen.devicePixelRatio
            sourceSize.height: height * Screen.devicePixelRatio
            fillMode: Image.PreserveAspectFit
        }

        scale: backMouseArea.containsMouse ? 1.3 : 1.0

        Behavior on scale
        {
            NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
        }

        MouseArea
        {
            id: backMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked:
            {
                root.backRequested()
            }
        }
    }

    // Main content
    Column
    {
        anchors.centerIn: parent
        spacing: 20
        width: 320

        Text
        {
            text: "Log In"
            font.family: "Segoe UI"
            font.pixelSize: 28
            font.weight: Font.Black
            color: "#101828"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text
        {
            text: "Please enter your details to sign in"
            font.family: "Segoe UI"
            font.pixelSize: 14
            color: "#6a748b"
            anchors.horizontalCenter: parent.horizontalCenter
            bottomPadding: 10
        }

        // Email
        TextField
        {
            id: emailField
            width: parent.width
            placeholderText: "Email address"
            font.family: "Segoe UI"
            font.pixelSize: 14
            color: "#101828"
            leftPadding: 16
            rightPadding: 16
            topPadding: 12
            bottomPadding: 12

            background: Rectangle
            {
                radius: 8
                border.color: emailField.activeFocus ? "#1a66ff" : "#e5e7eb"
                border.width: emailField.activeFocus ? 2 : 1
                color: "#ffffff"
            }
        }

        // Password
        TextField
        {
            id: passwordField
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

            background: Rectangle
            {
                radius: 8
                border.color: passwordField.activeFocus ? "#1a66ff" : "#e5e7eb"
                border.width: passwordField.activeFocus ? 2 : 1
                color: "#ffffff"
            }
        }

        // Sign In button
        Rectangle
        {
            width: parent.width
            height: 44
            radius: 8
            color: loginMouseArea.containsMouse ? "#0052cc" : "#1a66ff"

            Text
            {
                anchors.centerIn: parent
                text: "Sign In"
                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 16
                font.weight: Font.Bold
            }

            scale: loginMouseArea.containsMouse ? 1.3 : 1.0

            Behavior on scale
            {
                NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
            }


            MouseArea {
                id: loginMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked:
                {
                    root.loginSubmitted(emailField.text, passwordField.text)
                }
            }
        }
    }
}