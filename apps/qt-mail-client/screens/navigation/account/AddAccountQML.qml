import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Effects
//import "scripts/validator.js" as Validator

Rectangle
{
    id: rootWindow
    implicitWidth: 640
    implicitHeight: 460
    visible: true
    clip: true
    radius: 14

    function getValidationError(type, text) 
    {
        var value = text.trim();
        var value_lower = text.trim().toLowerCase()
        const layouts = 
        [
            "qwertyuiop", 
            "asdfghjkl", 
            "zxcvbnm", 
            "1234567890"
        ];

        if (type === "email") 
        {
            if (value.length === 0)
            {
                return "Email cannot be empty"
            }

            var email_regex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/
            if (!email_regex.test(value)) 
            {
                return "Invalid email format"
            }

            return ""
        }

        if (type === "password") 
        {
            if (value.length === 0) 
            {
                return "Password cannot be empty"
            }

            if (value.length < 6) 
            {
                return "Password must be at least 6 characters long"
            }

            if (value.length < 10) 
            {
                for (let layout of layouts) 
                {
                    for (let i = 0; i <= layout.length - 5; i++) 
                    {
                        let forward = layout.substring(i, i + 5);
                        let backward = forward.split("").reverse().join("");
            
                        if (value_lower.includes(forward) || value_lower.includes(backward)) 
                        {
                            return "Password cannot contain simple sequences";
                        }
                    }
                }
            }

            if (/(.)\1{4,}/.test(value)) 
            {
                return "Password cannot contain repeated characters";
            }

            if (!/[A-Z]/.test(value)) 
            {
                return "Password must contain at least one uppercase letter"
            }

            if (!/\d/.test(value)) 
            {
                return "Password must contain at least one number"
            }

            if (!/[!@#$%^&*(),.?":{}|<>\-_]/.test(value))
            {
                return "Password must contain at least one special character"
            }

            return ""
        }
        return ""
    }

    Rectangle
    {
        id: backgroundRectangle
        anchors.fill: parent
        color: "#ffffff"
        radius: 14
        border.color: "#e5e7eb"
        border.width: 1
    }

    Rectangle
    {
        id: closeButtonRectangle
        width: 40
        height: 40
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 4
        anchors.rightMargin: 4
        radius: 8
        color: "transparent"
        z: 10

        visible: !initialSetupRequired

        Image
        {
            id: closeIcon
            anchors.centerIn: parent
            source: "qrc:/pngs/assets/ic_close_window_black.svg"
            width: 15
            height: 15
            sourceSize.width: width * Screen.devicePixelRatio
            sourceSize.height: height * Screen.devicePixelRatio
            fillMode: Image.PreserveAspectFit
        }

        scale: closeClickArea.containsMouse ? 1.3 : 1.0

        Behavior on scale
        {
            id: closeScaleBehavior

            NumberAnimation
            {
                id: closeScaleAnimation
                duration: 150
                easing.type: Easing.InOutQuad
            }
        }

        MouseArea
        {
            id: closeClickArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onClicked:
            {
                closeAuthWindow()
            }
        }
    }

    // Dynamic screen loader for changing screens
    Loader
    {
        id: contentLoader
        anchors.fill: parent
        sourceComponent: choiceScreenComponent
    }

    // Event routing from loaded screens
    Connections
    {
        id: loaderConnections
        target: contentLoader.item
        ignoreUnknownSignals: true

        // Handle back navigation
        function onBackRequested()
        {
            contentLoader.sourceComponent = choiceScreenComponent
        }

        // Handle login submit
        function onLoginSubmitted(email, password)
        {
            //rootWindow.close()
        }

        // Handle registration submit
        function onRegisterSubmitted(name, email, password)
        {
            var success = regHandler.registerUser(name, email, password);

            if (success)
            {
                var firstLetter = avatarInitial(name)

                accountModel.AddAccount(name, email, "", "#3b82f6", firstLetter, true)
                CurrentUser.Authorize(name, email, "")

                closeAuthWindow()
            }
            else
            {
                return -1;
            }
        }
    }

    Component
    {
        id: choiceScreenComponent

        Item
        {
            id: choiceScreenRootItem
            anchors.fill: parent

            Column
            {
                id: mainLayoutColumn
                anchors.centerIn: parent
                spacing: 30

                Text
                {
                    id: mainTitleText
                    text: "Add an Account"
                    font.family: "Segoe UI"
                    font.pixelSize: 32
                    font.weight: Font.Bold
                    color: "#000000"
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Row
                {
                    id: optionsRow
                    spacing: 24
                    anchors.horizontalCenter: parent.horizontalCenter

                    // Login selection
                    Rectangle
                    {
                        id: loginSelectionBlock
                        width: 260
                        height: 300
                        radius: 12
                        border.color: "#e5e7eb"
                        border.width: 1
                        color: "transparent"

                        Column
                        {
                            id: loginContentColumn
                            anchors.centerIn: parent
                            spacing: 16
                            width: parent.width - 32

                            Image
                            {
                                id: loginIcon
                                source: "qrc:/pngs/assets/ic_login.svg"
                                width: 80
                                height: 80
                                sourceSize.width: width * Screen.devicePixelRatio
                                sourceSize.height: height * Screen.devicePixelRatio
                                fillMode: Image.PreserveAspectFit
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Text
                            {
                                id: loginTitleText
                                text: "Existing User?"
                                font.family: "Segoe UI"
                                font.pixelSize: 22
                                font.weight: Font.Bold
                                color: "#6a748b"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Rectangle
                            {
                                id: loginButton
                                width: 160
                                height: 44
                                radius: 8
                                color: loginClickArea.containsMouse ? "#0052cc" : "#1a66ff"
                                anchors.horizontalCenter: parent.horizontalCenter

                                Text
                                {
                                    id: loginButtonText
                                    anchors.centerIn: parent
                                    text: "Log In"
                                    color: "#ffffff"
                                    font.family: "Segoe UI"
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                }

                                scale: loginClickArea.containsMouse ? 1.05 : 1.0

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

                                MouseArea
                                {
                                    id: loginClickArea
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
                                id: loginDescriptionText
                                text: "Access your personal \n or work profile"
                                font.family: "Segoe UI"
                                font.pixelSize: 14
                                color: "#6a748b"
                                horizontalAlignment: Text.AlignHCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }

                    // Redister selection
                    Rectangle
                    {
                        id: registerSelectionBlock
                        width: 260
                        height: 300
                        radius: 12
                        border.color: "#e5e7eb"
                        border.width: 1
                        color: "transparent"

                        Column
                        {
                            id: registerContentColumn
                            anchors.centerIn: parent
                            anchors.verticalCenterOffset: -2
                            spacing: 16
                            width: parent.width - 32

                            Image
                            {
                                id: registerIcon
                                source: "qrc:/pngs/assets/ic_add_account.svg"
                                width: 64
                                height: 64
                                sourceSize.width: width * Screen.devicePixelRatio
                                sourceSize.height: height * Screen.devicePixelRatio
                                fillMode: Image.PreserveAspectFit
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Text
                            {
                                id: registerTitleText
                                text: "New User?"
                                font.family: "Segoe UI"
                                font.pixelSize: 22
                                font.weight: Font.Bold
                                color: "#6a748b"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Rectangle
                            {
                                id: registerButton
                                width: 160
                                height: 44
                                radius: 8
                                color: registerClickArea.containsMouse ? "#0052cc" : "#1a66ff"
                                anchors.horizontalCenter: parent.horizontalCenter

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

                                scale: registerClickArea.containsMouse ? 1.05 : 1.0

                                Behavior on scale
                                {
                                    id: registerScaleBehavior

                                    NumberAnimation
                                    {
                                        id: registerScaleAnimation
                                        duration: 150
                                        easing.type: Easing.InOutQuad
                                    }
                                }

                                MouseArea
                                {
                                    id: registerClickArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor

                                    onClicked:
                                    {
                                        contentLoader.source = "RegistrationQML.qml"
                                    }
                                }
                            }

                            Text
                            {
                                id: registerDescriptionText
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