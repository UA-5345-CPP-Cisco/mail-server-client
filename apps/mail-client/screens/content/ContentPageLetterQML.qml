import QtQuick
import QtQuick.Shapes
import QtQuick.Controls
import QtQuick.Layouts

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

    //header
    Rectangle {
        id: header

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        height: 124
        color: "white"

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            height: 1
            color: "#e5e7eb"
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 12

            //
            // SUBJECT
            //

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 28

                text: contentPageLetterQML.letterTheme

                color: "#101828"

                font.family: "Segoe UI"
                font.pixelSize: 20
                font.weight: Font.Black

                elide: Text.ElideRight
                wrapMode: Text.NoWrap

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }

            //
            // SECOND ROW
            //

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 12

                //
                // Avatar
                //

                Rectangle {
                    Layout.preferredWidth: 36
                    Layout.preferredHeight: 36

                    radius: 18
                    color: "#2b7fff"

                    Text {
                        anchors.centerIn: parent

                        text: contentPageLetterQML.letterName.length
                            ? contentPageLetterQML.letterName.charAt(0).toUpperCase()
                            : ""

                        color: "white"

                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Black
                    }
                }

                //
                // Name + recipient
                //

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 0

                    Text {
                        Layout.fillWidth: true

                        text: contentPageLetterQML.letterName

                        color: "#101828"

                        font.family: "Segoe UI"
                        font.pixelSize: 14

                        elide: Text.ElideRight
                        wrapMode: Text.NoWrap
                    }

                    Text {
                        Layout.fillWidth: true

                        text: contentPageLetterQML.letterSendTo.length
                            ? "to " + contentPageLetterQML.letterSendTo
                            : "to me"

                        color: "#6a7282"

                        font.family: "Segoe UI"
                        font.pixelSize: 12

                        elide: Text.ElideRight
                        wrapMode: Text.NoWrap
                    }
                }

                //
                // Time
                //

                Text {
                    Layout.alignment: Qt.AlignVCenter

                    text: formatEmailTimeFull(contentPageLetterQML.letterTime)

                    color: "#6a7282"

                    font.family: "Segoe UI"
                    font.pixelSize: 12
                }

                //
                // Star
                //

                Item {
                    Layout.preferredWidth: 34
                    Layout.preferredHeight: 34

                    MouseArea {
                        id: clickAreaFavourite

                        anchors.fill: parent

                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: {
                            contentPageLetterQML.letterStarred =
                                !contentPageLetterQML.letterStarred
                            starClicked(contentPageLetterQML.letterStarred)
                        }
                    }

                    Image {
                        anchors.centerIn: parent

                        width: 18
                        height: 18

                        source: contentPageLetterQML.letterStarred
                            ? "qrc:/pngs/assets/ic_star_active.svg"
                            : "qrc:/pngs/assets/ic_star.svg"

                        fillMode: Image.PreserveAspectFit

                        scale: clickAreaFavourite.containsMouse ? 1.3 : 1.0

                        Behavior on scale {
                            NumberAnimation {
                                duration: 150
                                easing.type: Easing.InOutQuad
                            }
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
        anchors.top: header.bottom
        anchors.bottom: header_1.top

        clip: true
        color: "transparent"

        Flickable
        {
            id: contentHolder

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 24
            anchors.topMargin: 24
            anchors.rightMargin: 12
            anchors.bottomMargin: 24
            clip: true
            contentWidth: width
            contentHeight: Math.max(height, letterBody.implicitHeight)
            boundsBehavior: Flickable.StopAtBounds

            Text
            {
                id: letterBody

                width: contentHolder.width - 14
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

            ScrollBar.vertical: ScrollBar
            {
                id: letterScrollBar

                policy: contentHolder.contentHeight > contentHolder.height ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
                active: hovered || pressed || contentHolder.moving
                width: 6

                background: Rectangle
                {
                    color: "transparent"
                }

                contentItem: Rectangle
                {
                    implicitWidth: 2
                    radius: 2
                    color: letterScrollBar.pressed ? "#4a5565" : "#9ca3af"
                    opacity: letterScrollBar.policy === ScrollBar.AlwaysOff ? 0 : 0.8
                }
            }
        }
    }

    // Footer
    Rectangle
    {
        id: header_1

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
