import QtQuick

//declaration of QML
Rectangle
{
    id: navigationQML

    color: Color.background

    signal inboxClicked
    signal starredClicked
    signal sentClicked
    signal draftClicked

    // headerNavigation
    Rectangle
    {
        id: headerNavigation
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 60
        color: Color.background
        z: 10

        // Bottom border
        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 1
            color: Color.outline
        }

        // Avatar
        Rectangle
        {
            id: avatar
            x: 12
            y: 14
            height: 32
            width: 32
            color: CurrentUser.avatarPath !== "" ? Color.transparent : Color.avatar
            radius: 16
            clip: true

            Image
            {
                anchors.fill: parent
                source: CurrentUser.avatarPath
                visible: CurrentUser.avatarPath !== ""
                cache: false
                fillMode: Image.PreserveAspectCrop
            }
            Text
            {
                anchors.centerIn: parent
                visible: CurrentUser.avatarPath === ""
                color: Color.background
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Black
                text: avatarInitial(CurrentUser.username)
            }
        }

        // Content
        Rectangle
        {
            id: userInfoWrapper
            anchors.left: avatar.right
            anchors.leftMargin: 8
            anchors.right: accountChangeWrapper.left
            anchors.rightMargin: 4
            anchors.verticalCenter: parent.verticalCenter
            height: 36
            color: Color.transparent

            Rectangle
            {
                id: userName
                anchors.left: parent.left
                anchors.right: parent.right
                height: 20
                clip: true
                color: Color.transparent

                Text
                {
                    id: userNameText
                    height: 20
                    width: parent.width
                    color: Color.button
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    text: CurrentUser.username
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                }
            }

            Rectangle
            {
                id: userInfo
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: userName.bottom
                height: 16
                clip: true
                color: Color.transparent

                Text
                {
                    id: userEmailText
                    height: 16
                    width: parent.width
                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: CurrentUser.email
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                }
            }
        }

        // Wrapper for the Button and Loader
        Item
        {
            id: accountChangeWrapper
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 48

            Rectangle
            {
                id: buttonAccountChange
                anchors.centerIn: parent
                height: 24
                width: 24
                color: clickAreaAccountChange.pressed ? Color.hover : Color.transparent
                radius: 4

                Rectangle
                {
                    id: accounChangeIcon
                    anchors.centerIn: parent
                    height: 16
                    width: 16
                    clip: true
                    color: Color.transparent
                    scale: clickAreaAccountChange.hovered ? 1.5 : 1.0

                    Behavior on scale
                    {
                        NumberAnimation
                        {
                            duration: 150
                            easing.type: Easing.InOutQuad
                        }
                    }

                    Image
                    {
                        source: "qrc:/pngs/assets/ic_arrow_down.svg"
                        width: 18
                        height: 18
                        sourceSize.width: width * Screen.devicePixelRatio
                        sourceSize.height: height * Screen.devicePixelRatio
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                    }
                }

                HoverHandler
                {
                    id: clickAreaAccountChange
                    cursorShape: Qt.PointingHandCursor
                }

                TapHandler
                {
                    onTapped:
                    {
                        if (String(accountChangeLoader.source) === "")
                        {
                            accountChangeLoader.source = "account/SwitchAccountQML.qml"
                        }
                        else
                        {
                            accountChangeLoader.source = ""
                        }
                    }
                }
            }
        }
    }
    
    // Compose button
    Rectangle
    {
        id: composeEmailWrapper

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: headerNavigation.bottom

        height: 76
        color: Color.transparent

        Rectangle
        {
            id: buttonToComposeEmail

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 16

            height: 44

            color: hoverHandlerComposeEmails.hovered ? Color.buttonSpecialHover : Color.buttonSpecial
            radius: 10

            HoverHandler
            {
                id: hoverHandlerComposeEmails
                cursorShape: Qt.PointingHandCursor
            }

            TapHandler
            {
                onTapped:
                {
                    if (newMessageLoader.selectedItem !== null)
                    {
                        newMessageLoader.selectedItem = null
                        newMessageLoader.active = false
                        newMessageLoader.active = true
                        newMessageLoader.source = "screens/navigation/new_message/NewMessageQML.qml"
                    }
                    else if (String(newMessageLoader.source) === "")
                    {
                        newMessageLoader.selectedItem = null
                        newMessageLoader.active = true
                        newMessageLoader.source = "screens/navigation/new_message/NewMessageQML.qml"
                    }
                    else
                    {
                        newMessageLoader.source = ""
                        newMessageLoader.active = false
                    }
                }
            }

            Rectangle
            {
                id: composeEmailIcon

                x: 16
                y: 13
                height: 18
                width: 18
                clip: true
                color: Color.transparent

                Image
                {
                    source: "qrc:/pngs/assets/ic_compose.svg"
                    width: 18
                    height: 18
                    sourceSize.width: width * Screen.devicePixelRatio
                    sourceSize.height: height * Screen.devicePixelRatio
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Text
            {
                id: composeEmailText

                x: 42
                y: 10
                height: 24
                width: 69

                color: Color.background
                font.family: "Segoe UI"
                font.pixelSize: 16
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignHCenter
                lineHeight: 24
                lineHeightMode: Text.FixedHeight
                text: "Compose"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    // Navigation
    Rectangle
    {
        id: navigationWrapper

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: composeEmailWrapper.bottom
        anchors.bottom: footerNavigation.top

        color: Color.transparent

        // Inbox button
        Rectangle
        {
            id: buttonToOpenInbox

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            height: 40
            radius: 10
            property bool isInboxSelected: selectedFolder === "inbox"

            color: isInboxSelected ? "#dbdbdb" : (hoverHandlerInboxButton.hovered ? Color.highlight : Color.background)

            HoverHandler
            {
                id: hoverHandlerInboxButton
                cursorShape: Qt.PointingHandCursor
            }

            TapHandler
            {
                onTapped:
                {
                    window.selectedFolder = "inbox"
                    inboxClicked()
                }
            }

            Rectangle
            {
                id: inboxButtonIcon
                x: 12; y: 11
                height: 18; width: 18
                clip: true; color: Color.transparent

                Image
                {
                    source: "qrc:/pngs/assets/ic_inbox.svg"
                    width: 18
                    height: 18
                    sourceSize.width: width * Screen.devicePixelRatio
                    sourceSize.height: height * Screen.devicePixelRatio
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Rectangle
            {
                id: inboxButtonTextWrapper
                x: 42; y: 8
                height: 24

                color: Color.transparent

                Text
                {
                    id: inboxButtonText
                    height: 24; width: parent.width
                    color: Color.button
                    font.family: "Segoe UI"; font.pixelSize: 16; font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 24; lineHeightMode: Text.FixedHeight
                    text: "Inbox"; textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter; wrapMode: Text.Wrap
                }
            }

            Rectangle
            {
                id: amountOfInboxHolder
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                height: 16; width: 20
                color: Color.transparent

                Text
                {
                    id: amountOfInboxText
                    height: 16; width: parent.width
                    color: Color.secondaryText
                    font.family: "Segoe UI";
                    font.pixelSize: 12;
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 16;
                    lineHeightMode: Text.FixedHeight
                    text: inboxCount;
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // Sent button
        Rectangle
        {
            id: buttonToOpenSent

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            anchors.top: buttonToOpenInbox.bottom
            anchors.topMargin: 4
            property bool isSentSelected: selectedFolder === "sent"

            color: isSentSelected ? "#dbdbdb" : (hoverHandlerSentButton.hovered ? Color.highlight : Color.background)
            height: 40
            radius: 10

            HoverHandler
            {
                id: hoverHandlerSentButton
                cursorShape: Qt.PointingHandCursor
            }

            TapHandler
            {
                onTapped:
                {
                    window.selectedFolder = "sent"
                    sentClicked()
                }
            }

            // TEMP ICON
            Rectangle
            {
                id: sentButtonIcon
                x: 12; y: 11
                height: 18; width: 18
                clip: true; color: Color.transparent

                Image
                {
                    source: "qrc:/pngs/assets/ic_sent.svg"
                    width: 18
                    height: 18
                    sourceSize.width: width * Screen.devicePixelRatio
                    sourceSize.height: height * Screen.devicePixelRatio
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Rectangle
            {
                id: sentButtonTextWrapper
                x: 42; y: 8
                height: 24
                color: Color.transparent

                Text
                {
                    id: sentButtonText
                    height: 24; width: parent.width
                    color: Color.primaryText
                    font.family: "Segoe UI";
                    font.pixelSize: 16;
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 24;
                    lineHeightMode: Text.FixedHeight
                    text: "Sent";
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter; wrapMode: Text.Wrap
                }
            }

            Rectangle
            {
                id: amountOfSentHolder
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                height: 16; width: 20
                color: Color.transparent

                Text
                {
                    id: amountOfSentText
                    height: 16; width: parent.width
                    color: Color.secondaryText
                    font.family: "Segoe UI";
                    font.pixelSize: 12;
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 16;
                    lineHeightMode: Text.FixedHeight
                    text: sentCount;
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // Starred button
        Rectangle
        {
            id: buttonToOpenStarred

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            anchors.top: buttonToOpenSent.bottom
            anchors.topMargin: 4
            property bool isStarredSelected: selectedFolder === "starred"


            color: isStarredSelected ? "#dbdbdb" : (hoverHandlerStarredButton.hovered ? Color.highlight : Color.background)
            height: 40
            radius: 10

            HoverHandler
            {
                id: hoverHandlerStarredButton
                cursorShape: Qt.PointingHandCursor
            }

            TapHandler
            {
                onTapped:
                {
                    window.selectedFolder = "starred"
                    starredClicked()
                }
            }


            Rectangle
            {
                id: starredButtonIcon
                x: 12; y: 11
                height: 18; width: 18
                clip: true; color: Color.transparent

                Image
                {
                    source: "qrc:/pngs/assets/ic_star.svg"
                    width: 18
                    height: 18
                    sourceSize.width: width * Screen.devicePixelRatio
                    sourceSize.height: height * Screen.devicePixelRatio
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Rectangle
            {
                id: starredButtonTextWrapper
                x: 42; y: 8
                height: 24
                color: Color.transparent

                Text
                {
                    id: starredButtonText
                    height: 24; width: parent.width
                    color: Color.primaryText
                    font.family: "Segoe UI"; font.pixelSize: 16; font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 24; lineHeightMode: Text.FixedHeight
                    text: "Starred"; textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter; wrapMode: Text.Wrap
                }
            }

            Rectangle
            {
                id: amountOfStarredHolder
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                height: 16; width: 20
                color: Color.transparent

                Text
                {
                    id: amountOfStarredText
                    height: 16; width: parent.width
                    color: Color.secondaryText
                    font.family: "Segoe UI";
                    font.pixelSize: 12;
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 16;
                    lineHeightMode: Text.FixedHeight
                    text: starredCount;
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // Drafts button
        Rectangle
        {
            id: buttonToOpenDrafts

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            anchors.top: buttonToOpenStarred.bottom
            anchors.topMargin: 4
            property bool isDraftsSelected: selectedFolder === "drafts"

            color: isDraftsSelected ? "#dbdbdb" : (hoverHandlerDraftsButton.hovered ? Color.highlight : Color.background)
            height: 40
            radius: 10

            HoverHandler
            {
                id: hoverHandlerDraftsButton
                cursorShape: Qt.PointingHandCursor
            }

            TapHandler
            {
                onTapped:
                {
                    window.selectedFolder = "drafts"
                    draftClicked()
                }
            }


            Rectangle {
                id: draftsButtonIcon
                x: 12; y: 11
                height: 18; width: 18
                clip: true; color: Color.transparent
                Image
                {
                    source: "qrc:/pngs/assets/ic_drafts.svg"
                    width: 18
                    height: 18
                    sourceSize.width: width * Screen.devicePixelRatio
                    sourceSize.height: height * Screen.devicePixelRatio
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Rectangle
            {
                id: draftsButtonTextWrapper
                x: 42; y: 8
                height: 24
                color: Color.transparent

                Text
                {
                    id: draftsButtonText
                    height: 24; width: parent.width
                    color: Color.secondaryText
                    font.family: "Segoe UI"; font.pixelSize: 16; font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 24; lineHeightMode: Text.FixedHeight
                    text: "Drafts"; textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter; wrapMode: Text.Wrap
                }
            }

            Rectangle
            {
                id: amountOfDraftsHolder
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                height: 16; width: 20
                color: Color.transparent

                Text
                {
                    id: amountOfDraftsText
                    height: 16; width: parent.width
                    color: Color.secondaryText
                    font.family: "Segoe UI"; font.pixelSize: 12; font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 16;
                    lineHeightMode: Text.FixedHeight
                    text: draftsCount;
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    // Footer: Settings button
    Rectangle
    {
        id: footerNavigation

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: 60
        color: clickAreaSettings.hovered ? "#dbdbdb" : Color.background

        // Top border
        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 1
            color: Color.outline
        }

        // Settings button
        Rectangle
        {
            id: buttonToOpenSettings

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 12
            color: clickAreaSettings.hovered ? "#dbdbdb" : Color.background
            anchors.rightMargin: 12

            height: 36

            radius: 10

            HoverHandler
            {
                id: clickAreaSettings
                cursorShape: Qt.PointingHandCursor
            }

            TapHandler
            {
                onTapped:
                {
                    settingsLoader.active = true
                    if (String( settingsLoader.source) === "")
                    {
                         settingsLoader.source = "screens/navigation/settings/SettingsQML.qml"
                    } else
                    {
                         settingsLoader.source = ""
                    }
                }
            }

            Text
            {
                id: settingsButtonText
                height: 20
                color: Color.secondaryText
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenterOffset: 0
                anchors.centerIn: parent
                text: "Settings"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter

                Rectangle
                {
                    id: settingsButtonIcon
                    y: 2
                    height: 16; width: 16
                    clip: true; color: Color.transparent
                    anchors.right: parent.left
                    anchors.rightMargin: 5
                    rotation: clickAreaSettings.hovered ? 180 : 0
                    Behavior on rotation
                    {
                        RotationAnimation
                        {
                            duration: 500
                            direction: RotationAnimation.Clockwise
                            easing.type: Easing.InOutQuad
                        }
                    }

                    Image
                    {
                        source: "qrc:/pngs/assets/ic_settings.svg"
                        width: 16
                        height: 16
                        sourceSize.width: width * Screen.devicePixelRatio
                        sourceSize.height: height * Screen.devicePixelRatio
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                    }
                }
            }
        }
    }

    // Loader for Account change popup
    Loader
    {
        id: accountChangeLoader
        z: 999
        anchors.top: headerNavigation.bottom
        anchors.topMargin: 4
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 0
        anchors.rightMargin: 0

        height: item ? item.implicitHeight : 0
        source: ""
        opacity: status === Loader.Ready ? 1 : 0

        Behavior on opacity
        {
            NumberAnimation { duration: 200 }
        }
    }
}
