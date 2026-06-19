import QtQuick
import QtQuick.Shapes

Rectangle
{
    id: contentPageLetterQML

    color: "#ffffff"
    property string letterIndex: ""
    property string letterTheme: "ThemeHolder"
    property string letterName: "NameHolder"
    property string letterSendTo: "me"
    property string letterContent: ""
    property string letterTime: "10:30"
    property bool letterStarred: false

    signal deleteClicked()
    signal starClicked(bool starred)

    // Header
    Rectangle
    {
        id: horizontalBorder

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: 124
        color: "#ffffff"

        // Bottom border
        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 1
            color: "#e5e7eb"
        }

        Rectangle
        {
            id: container_1

            anchors
            {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: 24
            }

            height: 76
            color: "transparent"

            // Content: theme and sender
            Rectangle
            {
                id: container_2

                anchors.left: parent.left
                anchors.right: container_6.left
                anchors.rightMargin: 8
                height: 76
                color: "transparent"

                // Theme
                Rectangle
                {
                    id: themeHolder

                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 28
                    color: "transparent"

                    Text
                    {
                        id: themeHolder_1

                        height: 28
                        width: parent.width

                        color: "#101828"
                        font.family: "Segoe UI"
                        font.pixelSize: 20
                        font.weight: Font.Black
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 28
                        lineHeightMode: Text.FixedHeight
                        text: contentPageLetterQML.letterTheme
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }

                // Sender
                Rectangle
                {
                    id: container_3

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: themeHolder.bottom
                    anchors.topMargin: 12
                    height: 36
                    color: "transparent"

                    // Icon
                    Rectangle
                    {
                        id: background

                        height: 36
                        width: 36
                        anchors.verticalCenter: parent.verticalCenter

                        color: "#2b7fff"
                        radius: 18

                        Text
                        {
                            id: s

                            anchors.centerIn: parent
                            height: 20
                            width: 10

                            color: "#ffffff"
                            font.family: "Segoe UI"
                            font.pixelSize: 14
                            font.weight: Font.Black
                            horizontalAlignment: Text.AlignHCenter
                            lineHeight: 20
                            lineHeightMode: Text.FixedHeight
                            text: contentPageLetterQML.letterName.length > 0 ? contentPageLetterQML.letterName.charAt(0).toUpperCase() : ""
                            textFormat: Text.PlainText
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    // Name To
                    Rectangle
                    {
                        id: container_4

                        anchors.left: background.right
                        anchors.leftMargin: 12
                        height: 36
                        width: 160
                        color: "transparent"

                        Rectangle
                        {
                            id: nameHolder

                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 20
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
                                text: contentPageLetterQML.letterName
                                textFormat: Text.PlainText
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        Rectangle
                        {
                            id: container_5

                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: nameHolder.bottom
                            height: 16
                            color: "transparent"

                            Text
                            {
                                id: to_me

                                height: 16
                                width: 33

                                color: "#6a7282"
                                font.family: "Segoe UI"
                                font.pixelSize: 12
                                font.weight: Font.Normal
                                horizontalAlignment: Text.AlignLeft
                                lineHeight: 16
                                lineHeightMode: Text.FixedHeight
                                text: contentPageLetterQML.letterSendTo.length > 0 ? "to " + contentPageLetterQML.letterSendTo : "to me"
                                textFormat: Text.PlainText
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }
            }

            // Right part
            Rectangle
            {
                id: container_6

                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: 34
                height: 34
                width: 131
                color: "transparent"

                // Time
                Rectangle
                {
                    id: timeHolder

                    anchors.left: parent.left
                    anchors.leftMargin: -8
                    anchors.verticalCenter: parent.verticalCenter
                    height: 16
                    width: 55
                    anchors.rightMargin:5
                    anchors.verticalCenterOffset: 1
                    color: "transparent"

                    Text
                    {
                        id: aM
                        x: 14
                        y: 0

                        height: 16
                        width: 52

                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 16
                        lineHeightMode: Text.FixedHeight
                        text: format_email_time_full(contentPageLetterQML.letterTime)
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                // Star
                Rectangle
                {
                    id: buttonToFavourite

                    anchors.right: button.left
                    anchors.rightMargin: 4
                    height: 34
                    width: 34
                    color: "transparent"
                    radius: 10

                    Rectangle
                    {
                        id: sVG

                        x: 8;
                        y: 8
                        height: 18;
                        width: 18
                        clip: true;
                        color: "transparent"
                        MouseArea
                        {
                            id: clickAreaFavourite
                            anchors.fill: parent

                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor

                            onClicked:
                            {
                                contentPageLetterQML.letterStarred = !contentPageLetterQML.letterStarred
                                starClicked(contentPageLetterQML.letterStarred)
                            }
                        }
                        scale: clickAreaFavourite.containsMouse ? 1.3 : 1.0

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
                            source: contentPageLetterQML.letterStarred ? "qrc:/pngs/assets/ic_star_active.svg" : "qrc:/pngs/assets/ic_star.svg"
                            width: 18
                            height: 18
                            sourceSize.width: width * Screen.devicePixelRatio
                            sourceSize.height: height * Screen.devicePixelRatio
                            fillMode: Image.PreserveAspectFit
                            anchors.centerIn: parent
                        }
                    }
                }

                // Other button
                Rectangle
                {
                    id: button

                    anchors.right: parent.right
                    height: 34
                    width: 34
                    color: "transparent"
                    radius: 10
                    MouseArea
                    {
                        id: clickAreaThreeDots
                        anchors.fill: parent

                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked:
                        {
                            // ContextMenu
                        }
                    }

                    Rectangle
                    {
                        id: sVG_1

                        x: 8;
                        y: 8
                        height: 18;
                        width: 18
                        clip: true;
                        color: "transparent"
                        scale: clickAreaThreeDots.containsMouse ? 1.5 : 1.0

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
                            source: "qrc:/pngs/assets/ic_three_dots.svg"
                            width: 18
                            height: 18
                            sourceSize.width: width * Screen.devicePixelRatio
                            sourceSize.height: height * Screen.devicePixelRatio
                            fillMode: Image.PreserveAspectFit
                            anchors.centerIn: parent
                        }
                    }
                }
            }
        }
    }

    // Main content
    Rectangle
    {
        id: container_7

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: horizontalBorder.bottom
        anchors.bottom: horizontalBorder_1.top

        clip: true
        color: "transparent"

        Rectangle
        {
            id: contentHolder

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 24
            color: "transparent"

            Text
            {
                id: letterBody

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                color: "#4a5565"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                lineHeight: 22.75
                lineHeightMode: Text.FixedHeight
                text: contentPageLetterQML.letterContent
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignTop
                wrapMode: Text.Wrap
            }
        }
    }

    // Footer
    Rectangle
    {
        id: horizontalBorder_1

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: 72
        color: "#ffffff"

        // Top border
        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 1
            color: "#e5e7eb"
        }

        // Reply button
        Rectangle
        {
            id: buttonToReply

            anchors.left: parent.left
            anchors.leftMargin: 16
            anchors.verticalCenter: parent.verticalCenter

            height: 38
            width: 66

            color: "#155dfc"
            radius: 10

            MouseArea
            {
                id:clickAreaReply
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked:
                {
                    console.log("Reply clicked")
                    // add reply logic here
                }
            }
            scale: clickAreaReply.containsMouse ? 1.1 : 1.0

            Behavior on scale
            {
                NumberAnimation
                {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            Text
            {
                id: reply

                anchors.centerIn: parent
                height: 20
                width: 35

                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignHCenter
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                text: "Reply"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
            }
        }

        // Forward button
        Rectangle
        {
            id: buttonToForward

            anchors.left: buttonToReply.right
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter

            height: 38
            width: 85

            border.color: "#e5e7eb"
            border.width: 1
            color: "transparent"
            radius: 10

            MouseArea
            {
                id:clickAreaForward
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked:
                {
                    console.log("Forward clicked")
                    // add forward logic here
                }
            }
            scale: clickAreaForward.containsMouse ? 1.1 : 1.0

            Behavior on scale
            {
                NumberAnimation
                {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }


            Text
            {
                id: forward

                anchors.centerIn: parent
                height: 20
                width: 52

                color: "#4a5565"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignHCenter
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                text: "Forward"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
            }
        }

        // Archive and Delete button
        Rectangle
        {
            id: container_8
            x: 568

            anchors.right: parent.right
            anchors.rightMargin: 14
            anchors.verticalCenterOffset: 5
            anchors.verticalCenter: parent.verticalCenter

            height: 28
            width: 58
            color: "transparent"

            // Archive
            Rectangle
            {
                id: buttonToArchiveEmail

                anchors.left: parent.left
                height: 18
                width: 18
                color: "transparent"
                radius: 10
                MouseArea
                {
                    id: clickAreaArchive
                    anchors.fill: parent

                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked:
                    {
                        // MinimizeWindow
                    }
                }
                scale: clickAreaArchive.containsMouse ? 1.3: 1.0

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
                    source: "qrc:/pngs/assets/ic_archive.svg"
                    width: 18
                    height: 18
                    sourceSize.width: width * Screen.devicePixelRatio
                    sourceSize.height: height * Screen.devicePixelRatio
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            // Delete
            Rectangle
            {
                id: buttonToDeleteEmail

                anchors.right: parent.right
                height: 18
                width: 18
                color: "transparent"
                radius: 10

                MouseArea
                {
                    id: clickAreaDelete
                    anchors.fill: parent

                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked:
                    {
                        console.log("INDEX: " + letterIndex)
                        deleteClicked()
                    }
                }
                scale: clickAreaDelete.containsMouse ? 1.3 : 1.0

                Behavior on scale
                {
                    NumberAnimation
                    {
                        duration: 150
                        easing.type: Easing.InOutQuad
                    }
                }
                // TEMP ICON
                Image
                {
                    source: "qrc:/pngs/assets/ic_bin.svg"
                    width: 18
                    height: 18
                    sourceSize.width: width * Screen.devicePixelRatio
                    sourceSize.height: height * Screen.devicePixelRatio
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }
        }
    }
}
