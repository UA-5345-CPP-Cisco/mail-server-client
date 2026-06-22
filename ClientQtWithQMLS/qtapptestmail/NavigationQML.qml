import QtQuick
import QtQuick.Effects
import QtQuick.Shapes
import QtQuick.Controls
Rectangle
{
    id: navigationQML

    color: "#ffffff"

    signal inboxClicked
    signal starredClicked
    signal sentClicked
    signal draftClicked

    function avatarInitial(name)
    {
        var trimmedName = String(name).trim()
        return trimmedName.length > 0 ? trimmedName.charAt(0).toUpperCase() : "?"
    }

    // Header
    Rectangle
    {
        id: horizontalBorder

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: 60
        color: "#ffffff"

        z: 10


        // Bottom border
        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 1
            color: "#e5e7eb"
        }

        //Avatar
        Rectangle
        {
            id: avatar

            x: 12
            y: 14
            height: 32
            width: 32

            color: currentUser.avatarPath !== "" ? "transparent" : "#2b7fff"
            radius: 16
            clip: true

            Image
            {
                anchors.fill: parent
                source: currentUser.avatarPath
                visible: currentUser.avatarPath !== ""
                cache: false
                fillMode: Image.PreserveAspectCrop
            }

            Text
            {
                anchors.centerIn: parent
                visible: currentUser.avatarPath === ""
                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Black
                text: avatarInitial(currentUser.username)
            }
        }


        // Content
        Rectangle
        {
            id: container

            anchors.left: avatar.right
            anchors.leftMargin: 8
            anchors.right: dropdownWrapper.left
            anchors.rightMargin: 4
            anchors.verticalCenter: parent.verticalCenter
            height: 36
            color: "transparent"

            Rectangle
            {
                id: nameHolder

                anchors.left: parent.left
                anchors.right: parent.right
                height: 20
                clip: true
                color: "transparent"

                Text
                {
                    id: nameHolder_1

                    height: 20
                    width: parent.width

                    color: "#101828"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    text: currentUser.username
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                }
            }

            Rectangle
            {
                id: emailHolder

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: nameHolder.bottom
                height: 16
                clip: true
                color: "transparent"

                Text
                {
                    id: emailHolder_1

                    height: 16
                    width: parent.width

                    color: "#6a7282"
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: currentUser.email
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                }
            }
        }

        // Wrapper for the Button and Loader
        Item
        {
            id: dropdownWrapper

            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 48

            Rectangle
            {
                id: buttonBurgerToPullEmails

                anchors.centerIn: parent
                height: 24
                width: 24

                color: clickAreaPullEmails.pressed ? "#ffdede" : "transparent"
                radius: 4

                Rectangle
                {
                    id: sVG
                    anchors.centerIn: parent
                    height: 16
                    width: 16
                    clip: true
                    color: "transparent"

                    scale: clickAreaPullEmails.hovered ? 1.5 : 1.0

                    Behavior on scale {
                        NumberAnimation {
                            duration: 150
                            easing.type: Easing.InOutQuad
                        }
                    }

                    Image
                    {
                        source: "qrc:/pngs/assets/ic_arrow_down.svg"
                        width: 18
                        height: 18
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                    }
                }

                HoverHandler
                {
                    id: clickAreaPullEmails
                    cursorShape: Qt.PointingHandCursor
                }

                TapHandler
                {
                    onTapped:
                    {
                        if (String(accountChangeLoader.source) === "")
                        {
                            accountChangeLoader.source = "SwitchAccountQML.qml"
                        } else
                        {
                            accountChangeLoader.source = ""
                        }
                    }
                }
            }
        }

        //Loader for Account change popup
        Loader
        {
            id: accountChangeLoader
            z: 999
            anchors.top: parent.bottom
            anchors.topMargin: 4
            anchors.left: background.left
            anchors.leftMargin: -12

            width: item ? item.implicitWidth : 0
            height: item ? item.implicitHeight : 0

            source: ""
            Behavior on opacity
            {
                NumberAnimation { duration: 200 }
            }

            opacity: status === Loader.Ready ? 1 : 0
        }
}

    // Compose button
    Rectangle
    {
        id: container_1

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: horizontalBorder.bottom

        height: 76
        color: "transparent"

        Rectangle
        {
            id: buttonToComposeEmail

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 16

            height: 44

            color: hoverHandlerComposeEmails.hovered ? "#0c43f7" : "#155dfc"
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
                        newMessageLoader.active = true
                        if (String(newMessageLoader.source) === "")
                        {
                            newMessageLoader.source = "NewMessageQML.qml"
                        } else
                        {
                            newMessageLoader.source = ""
                        }
                    }
                }

            Rectangle
            {
                id: sVG_1

                x: 16
                y: 13
                height: 18
                width: 18
                clip: true
                color: "transparent"

                Image
                {
                    source: "qrc:/pngs/assets/ic_compose.svg"
                    width: 18
                    height: 18
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Text
            {
                id: compose

                x: 42
                y: 10
                height: 24
                width: 69

                color: "#ffffff"
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
        id: nav

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: container_1.bottom
        anchors.bottom: footer.top

        color: "transparent"

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

            color: isInboxSelected ? "#dbdbdb" : (hoverHandlerInboxButton.hovered ? "#f3f4f6" : "#ffffff")

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

            // TEMP ICON
            Rectangle
            {
                id: sVG_2
                x: 12; y: 11
                height: 18; width: 18
                clip: true; color: "transparent"

                Image
                {
                    source: "qrc:/pngs/assets/ic_inbox.svg"
                    width: 18
                    height: 18
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Rectangle
            {
                id: container_2
                x: 42; y: 8
                height: 24

                color: "transparent"

                Text
                {
                    id: inbox
                    height: 24; width: parent.width
                    color: "#101828"
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
                color: "transparent"

                Text
                {
                    id: element
                    height: 16; width: parent.width
                    color: "#6a7282"
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

            color: isSentSelected ? "#dbdbdb" : (hoverHandlerSentButton.hovered ? "#f3f4f6" : "#ffffff")
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
                id: sVG_4
                x: 12; y: 11
                height: 18; width: 18
                clip: true; color: "transparent"

                Image
                {
                    source: "qrc:/pngs/assets/ic_sent.svg"
                    width: 18
                    height: 18
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Rectangle
            {
                id: container_4
                x: 42; y: 8
                height: 24
                color: "transparent"

                Text
                {
                    id: sent
                    height: 24; width: parent.width
                    color: "#4a5565"
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
                color: "transparent"

                Text
                {
                    id: element_2
                    height: 16; width: parent.width
                    color: "#6a7282"
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


            color: isStarredSelected ? "#dbdbdb" : (hoverHandlerStarredButton.hovered ? "#f3f4f6" : "#ffffff")
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

            // TEMP ICON
            Rectangle
            {
                id: sVG_3
                x: 12; y: 11
                height: 18; width: 18
                clip: true; color: "transparent"

                Image
                {
                    source: "qrc:/pngs/assets/ic_star.svg"
                    width: 18
                    height: 18
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Rectangle
            {
                id: container_3
                x: 42; y: 8
                height: 24
                color: "transparent"

                Text
                {
                    id: starred
                    height: 24; width: parent.width
                    color: "#4a5565"
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
                color: "transparent"

                Text
                {
                    id: element_1
                    height: 16; width: parent.width
                    color: "#6a7282"
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

            color: isDraftsSelected ? "#dbdbdb" : (hoverHandlerDraftsButton.hovered ? "#f3f4f6" : "#ffffff")
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

            // TEMP ICON
            Rectangle {
                id: sVG_5
                x: 12; y: 11
                height: 18; width: 18
                clip: true; color: "transparent"
                Image
                {
                    source: "qrc:/pngs/assets/ic_drafts.svg"
                    width: 18
                    height: 18
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Rectangle
            {
                id: container_5
                x: 42; y: 8
                height: 24
                color: "transparent"

                Text
                {
                    id: drafts
                    height: 24; width: parent.width
                    color: "#4a5565"
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
                color: "transparent"

                Text
                {
                    id: element_3
                    height: 16; width: parent.width
                    color: "#6a7282"
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
        id: footer

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: 60
        color: clickAreaSettings.hovered ? "#dbdbdb" : "#ffffff"
        // Top border
        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 1
            color: "#e5e7eb"
        }

        // Settings button
        Rectangle
        {
            id: buttonToOpenSettings

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 12
            color: clickAreaSettings.hovered ? "#dbdbdb" : "#ffffff"
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
                             settingsLoader.source = "SettingsQML.qml"
                        } else
                        {
                             settingsLoader.source = ""
                        }
                    }
            }

            Text
            {
                id: settings
                height: 20
                color: "#4a5565"
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

                // TEMP ICON
                Rectangle
                {
                    id: sVG_6
                    y: 2
                    height: 16; width: 16
                    clip: true; color: "transparent"
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
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                    }
                }
            }
        }
    }
}
