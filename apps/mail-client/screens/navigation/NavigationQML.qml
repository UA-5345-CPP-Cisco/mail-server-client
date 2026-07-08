import QtQuick

//declaration of QML
Rectangle {
    id: navigationQML

    signal archiveClicked
    signal draftClicked
    signal inboxClicked
    signal sentClicked
    signal starredClicked

    color: Color.background

    // headerNavigation
    Rectangle {
        id: headerNavigation

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        color: Color.background
        height: 60
        z: 10

        // Bottom border
        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            color: Color.outline
            height: 1
        }

        // Avatar
        Rectangle {
            id: avatar

            clip: true
            color: CurrentUser.avatarPath !== "" ? Color.transparent : Color.avatar
            height: 32
            radius: 16
            width: 32
            x: 12
            y: 14

            Image {
                anchors.fill: parent
                cache: false
                fillMode: Image.PreserveAspectCrop
                source: CurrentUser.avatarPath
                visible: CurrentUser.avatarPath !== ""
            }
            Text {
                anchors.centerIn: parent
                color: Color.background
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Black
                text: avatarInitial(CurrentUser.username)
                visible: CurrentUser.avatarPath === ""
            }
        }

        // Content
        Rectangle {
            id: userInfoWrapper

            anchors.left: avatar.right
            anchors.leftMargin: 8
            anchors.right: accountChangeWrapper.left
            anchors.rightMargin: 4
            anchors.verticalCenter: parent.verticalCenter
            color: Color.transparent
            height: 36

            Rectangle {
                id: userName

                anchors.left: parent.left
                anchors.right: parent.right
                clip: true
                color: Color.transparent
                height: 20

                Text {
                    id: userNameText

                    color: Color.button
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    height: 20
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    text: CurrentUser.username
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    wrapMode: Text.Wrap
                }
            }
            Rectangle {
                id: userInfo

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: userName.bottom
                clip: true
                color: Color.transparent
                height: 16

                Text {
                    id: userEmailText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    height: 16
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: CurrentUser.email
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    wrapMode: Text.Wrap
                }
            }
        }

        // Wrapper for the Button and Loader
        Item {
            id: accountChangeWrapper

            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.top: parent.top
            width: 48

            Rectangle {
                id: buttonAccountChange

                anchors.centerIn: parent
                color: clickAreaAccountChange.pressed ? Color.hover : Color.transparent
                height: 24
                radius: 4
                width: 24

                Rectangle {
                    id: accounChangeIcon

                    anchors.centerIn: parent
                    clip: true
                    color: Color.transparent
                    height: 16
                    scale: clickAreaAccountChange.hovered ? 1.5 : 1.0
                    width: 16

                    Behavior on scale {
                        NumberAnimation {
                            duration: 150
                            easing.type: Easing.InOutQuad
                        }
                    }

                    Image {
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        height: 18
                        source: "qrc:/pngs/assets/ic_arrow_down.svg"
                        sourceSize.height: height * Screen.devicePixelRatio
                        sourceSize.width: width * Screen.devicePixelRatio
                        width: 18
                    }
                }
                HoverHandler {
                    id: clickAreaAccountChange

                    cursorShape: Qt.PointingHandCursor
                }
                TapHandler {
                    onTapped: {
                        if (String(accountChangeLoader.source) === "") {
                            accountChangeLoader.source = "account/SwitchAccountQML.qml";
                        } else {
                            accountChangeLoader.source = "";
                        }
                    }
                }
            }
        }
    }

    // Compose button
    Rectangle {
        id: composeEmailWrapper

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: headerNavigation.bottom
        color: Color.transparent
        height: 76

        Rectangle {
            id: buttonToComposeEmail

            anchors.left: parent.left
            anchors.margins: 16
            anchors.right: parent.right
            anchors.top: parent.top
            color: hoverHandlerComposeEmails.hovered ? Color.buttonSpecialHover : Color.buttonSpecial
            height: 44
            radius: 10

            HoverHandler {
                id: hoverHandlerComposeEmails

                cursorShape: Qt.PointingHandCursor
            }
            TapHandler {
                onTapped: {
                    if (newMessageLoader.selectedItem !== null) {
                        newMessageLoader.selectedItem = null;
                        newMessageLoader.active = false;
                        newMessageLoader.active = true;
                        newMessageLoader.source = "screens/navigation/new_message/NewMessageQML.qml";
                    } else if (String(newMessageLoader.source) === "") {
                        newMessageLoader.selectedItem = null;
                        newMessageLoader.active = true;
                        newMessageLoader.source = "screens/navigation/new_message/NewMessageQML.qml";
                    } else {
                        newMessageLoader.source = "";
                        newMessageLoader.active = false;
                    }
                }
            }
            Rectangle {
                id: composeEmailIcon

                clip: true
                color: Color.transparent
                height: 18
                width: 18
                x: 16
                y: 13

                Image {
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    height: 18
                    source: "qrc:/pngs/assets/ic_compose.svg"
                    sourceSize.height: height * Screen.devicePixelRatio
                    sourceSize.width: width * Screen.devicePixelRatio
                    width: 18
                }
            }
            Text {
                id: composeEmailText

                color: Color.buttonSpecialText
                font.family: "Segoe UI"
                font.pixelSize: 16
                font.weight: Font.Normal
                height: 24
                horizontalAlignment: Text.AlignHCenter
                lineHeight: 24
                lineHeightMode: Text.FixedHeight
                text: "Compose"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
                width: 69
                x: 42
                y: 10
            }
        }
    }

    // Navigation
    Rectangle {
        id: navigationWrapper

        anchors.bottom: footerNavigation.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: composeEmailWrapper.bottom
        color: Color.transparent

        // Inbox button
        Rectangle {
            id: buttonToOpenInbox

            property bool isInboxSelected: selectedFolder === "inbox"

            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
            color: isInboxSelected ? Color.selected : (hoverHandlerInboxButton.hovered ? Color.highlight : Color.background)
            height: 40
            radius: 10

            HoverHandler {
                id: hoverHandlerInboxButton

                cursorShape: Qt.PointingHandCursor
            }
            TapHandler {
                onTapped: {
                    window.selectedFolder = "inbox";
                    inboxClicked();
                }
            }
            Rectangle {
                id: inboxButtonIcon

                clip: true
                color: Color.transparent
                height: 18
                width: 18
                x: 12
                y: 11

                Image {
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    height: 18
                    source: "qrc:/pngs/assets/ic_inbox.svg"
                    sourceSize.height: height * Screen.devicePixelRatio
                    sourceSize.width: width * Screen.devicePixelRatio
                    width: 18
                }
            }
            Rectangle {
                id: inboxButtonTextWrapper

                color: Color.transparent
                height: 24
                x: 42
                y: 8

                Text {
                    id: inboxButtonText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    height: 24
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 24
                    lineHeightMode: Text.FixedHeight
                    text: "Inbox"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    wrapMode: Text.Wrap
                }
            }
            Rectangle {
                id: amountOfInboxHolder

                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                color: Color.transparent
                height: 16
                width: 20

                Text {
                    id: amountOfInboxText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    height: 16
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: inboxCount
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                }
            }
        }

        // Sent button
        Rectangle {
            id: buttonToOpenSent

            property bool isSentSelected: selectedFolder === "sent"

            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.top: buttonToOpenInbox.bottom
            anchors.topMargin: 4
            color: isSentSelected ? Color.selected : (hoverHandlerSentButton.hovered ? Color.highlight : Color.background)
            height: 40
            radius: 10

            HoverHandler {
                id: hoverHandlerSentButton

                cursorShape: Qt.PointingHandCursor
            }
            TapHandler {
                onTapped: {
                    window.selectedFolder = "sent";
                    sentClicked();
                }
            }

            // TEMP ICON
            Rectangle {
                id: sentButtonIcon

                clip: true
                color: Color.transparent
                height: 18
                width: 18
                x: 12
                y: 11

                Image {
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    height: 18
                    source: "qrc:/pngs/assets/ic_sent.svg"
                    sourceSize.height: height * Screen.devicePixelRatio
                    sourceSize.width: width * Screen.devicePixelRatio
                    width: 18
                }
            }
            Rectangle {
                id: sentButtonTextWrapper

                color: Color.transparent
                height: 24
                x: 42
                y: 8

                Text {
                    id: sentButtonText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    height: 24
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 24
                    lineHeightMode: Text.FixedHeight
                    text: "Sent"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    wrapMode: Text.Wrap
                }
            }
            Rectangle {
                id: amountOfSentHolder

                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                color: Color.transparent
                height: 16
                width: 20

                Text {
                    id: amountOfSentText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    height: 16
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: sentCount
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                }
            }
        }

        // Starred button
        Rectangle {
            id: buttonToOpenStarred

            property bool isStarredSelected: selectedFolder === "starred"

            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.top: buttonToOpenSent.bottom
            anchors.topMargin: 4
            color: isStarredSelected ? Color.selected : (hoverHandlerStarredButton.hovered ? Color.highlight : Color.background)
            height: 40
            radius: 10

            HoverHandler {
                id: hoverHandlerStarredButton

                cursorShape: Qt.PointingHandCursor
            }
            TapHandler {
                onTapped: {
                    window.selectedFolder = "starred";
                    starredClicked();
                }
            }
            Rectangle {
                id: starredButtonIcon

                clip: true
                color: Color.transparent
                height: 18
                width: 18
                x: 12
                y: 11

                Image {
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    height: 18
                    source: "qrc:/pngs/assets/ic_star.svg"
                    sourceSize.height: height * Screen.devicePixelRatio
                    sourceSize.width: width * Screen.devicePixelRatio
                    width: 18
                }
            }
            Rectangle {
                id: starredButtonTextWrapper

                color: Color.transparent
                height: 24
                x: 42
                y: 8

                Text {
                    id: starredButtonText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    height: 24
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 24
                    lineHeightMode: Text.FixedHeight
                    text: "Starred"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    wrapMode: Text.Wrap
                }
            }
            Rectangle {
                id: amountOfStarredHolder

                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                color: Color.transparent
                height: 16
                width: 20

                Text {
                    id: amountOfStarredText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    height: 16
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: starredCount
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                }
            }
        }

        // Drafts button
        Rectangle {
            id: buttonToOpenDrafts

            property bool isDraftsSelected: selectedFolder === "drafts"

            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.top: buttonToOpenStarred.bottom
            anchors.topMargin: 4
            color: isDraftsSelected ? Color.selected : (hoverHandlerDraftsButton.hovered ? Color.highlight : Color.background)
            height: 40
            radius: 10

            HoverHandler {
                id: hoverHandlerDraftsButton

                cursorShape: Qt.PointingHandCursor
            }
            TapHandler {
                onTapped: {
                    window.selectedFolder = "drafts";
                    draftClicked();
                }
            }
            Rectangle {
                id: draftsButtonIcon

                clip: true
                color: Color.transparent
                height: 18
                width: 18
                x: 12
                y: 11

                Image {
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    height: 18
                    source: "qrc:/pngs/assets/ic_drafts.svg"
                    sourceSize.height: height * Screen.devicePixelRatio
                    sourceSize.width: width * Screen.devicePixelRatio
                    width: 18
                }
            }
            Rectangle {
                id: draftsButtonTextWrapper

                color: Color.transparent
                height: 24
                x: 42
                y: 8

                Text {
                    id: draftsButtonText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    height: 24
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 24
                    lineHeightMode: Text.FixedHeight
                    text: "Drafts"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    wrapMode: Text.Wrap
                }
            }
            Rectangle {
                id: amountOfDraftsHolder

                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                color: Color.transparent
                height: 16
                width: 20

                Text {
                    id: amountOfDraftsText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    height: 16
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: draftsCount
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                }
            }
        }
        Rectangle {
            id: buttonToOpenArchives

            property bool isArchivesSelected: selectedFolder === "archive"

            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.top: buttonToOpenDrafts.bottom
            anchors.topMargin: 4
            color: isArchivesSelected ? Color.selected : (hoverHandlerArchivesButton.hovered ? Color.highlight : Color.background)
            height: 40
            radius: 10

            HoverHandler {
                id: hoverHandlerArchivesButton

                cursorShape: Qt.PointingHandCursor
            }
            TapHandler {
                onTapped: {
                    window.selectedFolder = "archive";
                    archiveClicked();
                }
            }
            Rectangle {
                id: archivesButtonIcon

                clip: true
                color: Color.transparent
                height: 18
                width: 18
                x: 12
                y: 11

                Image {
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    height: 18
                    source: "qrc:/pngs/assets/ic_archive.svg"
                    sourceSize.height: height * Screen.devicePixelRatio
                    sourceSize.width: width * Screen.devicePixelRatio
                    width: 18
                }
            }
            Rectangle {
                id: archivesButtonTextWrapper

                color: Color.transparent
                height: 24
                x: 42
                y: 8

                Text {
                    id: archivesButtonText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    height: 24
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 24
                    lineHeightMode: Text.FixedHeight
                    text: "Archive"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    wrapMode: Text.Wrap
                }
            }
            Rectangle {
                id: amountOfArchivesHolder

                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                color: Color.transparent
                height: 16
                width: 20

                Text {
                    id: amountOfArchivesText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    height: 16
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: archiveCount
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                }
            }
        }
    }

    // Footer: Settings button
    Rectangle {
        id: footerNavigation

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        color: clickAreaSettings.hovered ? Color.selected : Color.background
        height: 60

        // Top border
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            color: Color.outline
            height: 1
        }

        // Settings button
        Rectangle {
            id: buttonToOpenSettings

            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.verticalCenter: parent.verticalCenter
            color: clickAreaSettings.hovered ? Color.selected : Color.background
            height: 36
            radius: 10

            HoverHandler {
                id: clickAreaSettings

                cursorShape: Qt.PointingHandCursor
            }
            TapHandler {
                onTapped: {
                    settingsLoader.active = true;
                    if (String(settingsLoader.source) === "") {
                        settingsLoader.source = "screens/navigation/settings/SettingsQML.qml";
                    } else {
                        settingsLoader.source = "";
                    }
                }
            }
            Text {
                id: settingsButtonText

                anchors.centerIn: parent
                anchors.horizontalCenterOffset: 0
                anchors.verticalCenterOffset: 0
                color: Color.secondaryText
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                height: 20
                horizontalAlignment: Text.AlignLeft
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                text: "Settings"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter

                Rectangle {
                    id: settingsButtonIcon

                    anchors.right: parent.left
                    anchors.rightMargin: 5
                    clip: true
                    color: Color.transparent
                    height: 16
                    rotation: clickAreaSettings.hovered ? 180 : 0
                    width: 16
                    y: 2

                    Behavior on rotation {
                        RotationAnimation {
                            direction: RotationAnimation.Clockwise
                            duration: 500
                            easing.type: Easing.InOutQuad
                        }
                    }

                    Image {
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        height: 16
                        source: "qrc:/pngs/assets/ic_settings.svg"
                        sourceSize.height: height * Screen.devicePixelRatio
                        sourceSize.width: width * Screen.devicePixelRatio
                        width: 16
                    }
                }
            }
        }
    }

    // Loader for Account change popup
    Loader {
        id: accountChangeLoader

        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: headerNavigation.bottom
        anchors.topMargin: 4
        height: item ? item.implicitHeight : 0
        opacity: status === Loader.Ready ? 1 : 0
        source: ""
        z: 999

        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
    }
}
