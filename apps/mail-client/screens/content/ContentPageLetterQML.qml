import QtQuick
import QtQuick.Shapes
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: contentPageLetterQML

    property string letterContent: ""
    property string letterIndex: ""
    property string letterName: "NameHolder"
    property string letterSendTo: "me"
    property bool letterStarred: false
    property string letterTheme: "ThemeHolder"
    property string letterTime: "10:30"

    signal deleteClicked
    signal starClicked(bool starred)

    color: Color.background

    //header
    Rectangle {
        id: header

        color: "white"
        height: 124

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        Rectangle {
            color: Color.outline
            height: 1

            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
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
                color: Color.button
                elide: Text.ElideRight
                font.family: "Segoe UI"
                font.pixelSize: 20
                font.weight: Font.Black
                horizontalAlignment: Text.AlignLeft
                text: contentPageLetterQML.letterTheme
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.NoWrap
            }

            //
            // SECOND ROW
            //

            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 12

                //
                // Avatar
                //

                Rectangle {
                    Layout.preferredHeight: 36
                    Layout.preferredWidth: 36
                    color: "#2b7fff"
                    radius: 18

                    Text {
                        anchors.centerIn: parent
                        color: "white"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Black
                        text: contentPageLetterQML.letterName.length ? contentPageLetterQML.letterName.charAt(0).toUpperCase() : ""
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
                        color: Color.button
                        elide: Text.ElideRight
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        text: contentPageLetterQML.letterName
                        wrapMode: Text.NoWrap
                    }
                    Text {
                        Layout.fillWidth: true
                        color: Color.secondaryText
                        elide: Text.ElideRight
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        text: contentPageLetterQML.letterSendTo.length ? "to " + contentPageLetterQML.letterSendTo : "to me"
                        wrapMode: Text.NoWrap
                    }
                }

                //
                // Time
                //

                Text {
                    Layout.alignment: Qt.AlignVCenter
                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    text: formatEmailTimeFull(contentPageLetterQML.letterTime)
                }

                //
                // Star
                //

                Item {
                    Layout.preferredHeight: 34
                    Layout.preferredWidth: 34

                    MouseArea {
                        id: clickAreaFavourite

                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true

                        onClicked: {
                            contentPageLetterQML.letterStarred = !contentPageLetterQML.letterStarred;
                            starClicked(contentPageLetterQML.letterStarred);
                        }
                    }
                    Image {
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        height: 18
                        scale: clickAreaFavourite.containsMouse ? 1.3 : 1.0
                        source: contentPageLetterQML.letterStarred ? "qrc:/pngs/assets/ic_star_active.svg" : "qrc:/pngs/assets/ic_star.svg"
                        width: 18

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
    Rectangle {
        id: container_7

        anchors.bottom: header_1.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        clip: true
        color: Color.transparent

        Flickable {
            id: contentHolder

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 24
            anchors.left: parent.left
            anchors.leftMargin: 24
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.top: parent.top
            anchors.topMargin: 24
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            contentHeight: Math.max(height, letterBody.implicitHeight)
            contentWidth: width

            ScrollBar.vertical: ScrollBar {
                id: letterScrollBar

                active: hovered || pressed || contentHolder.moving
                policy: contentHolder.contentHeight > contentHolder.height ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
                width: 6

                background: Rectangle {
                    color: Color.transparent
                }
                contentItem: Rectangle {
                    color: letterScrollBar.pressed ? "#4a5565" : "#9ca3af"
                    implicitWidth: 2
                    opacity: letterScrollBar.policy === ScrollBar.AlwaysOff ? 0 : 0.8
                    radius: 2
                }
            }

            Text {
                id: letterBody

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
                width: contentHolder.width - 14
                wrapMode: Text.Wrap
            }
        }
    }

    // Footer
    Rectangle {
        id: header_1

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        color: Color.background
        height: 72

        // Top border
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            color: Color.outline
            height: 1
        }

        // Reply button
        Rectangle {
            id: buttonToReply

            anchors.left: parent.left
            anchors.leftMargin: 16
            anchors.verticalCenter: parent.verticalCenter
            color: "#155dfc"
            height: 38
            radius: 10
            scale: clickAreaReply.containsMouse ? 1.1 : 1.0
            width: 66

            Behavior on scale {
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            MouseArea {
                id: clickAreaReply

                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: {
                    newMessageLoader.selectedItem = {
                        "index": letterIndex,
                        "subject": "re: " + letterTheme,
                        "sendTo": letterSendTo,
                        "content": "",
                        "IsReply": true,
                        "IsForward": false,
                        "isDraft": false,
                        "title": "Reply",
                        "newTitle": "Reply"
                    };

                    newMessageLoader.source = "";
                    newMessageLoader.active = false;

                    newMessageLoader.active = true;
                    newMessageLoader.source = "screens/navigation/new_message/NewMessageQML.qml";
                }
            }
            Text {
                id: reply

                anchors.centerIn: parent
                color: Color.background
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                height: 20
                horizontalAlignment: Text.AlignHCenter
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                text: "Reply"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
                width: 35
            }
        }

        // Forward button
        Rectangle {
            id: buttonToForward

            anchors.left: buttonToReply.right
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            border.color: Color.outline
            border.width: 1
            color: Color.transparent
            height: 38
            radius: 10
            scale: clickAreaForward.containsMouse ? 1.1 : 1.0
            width: 85

            Behavior on scale {
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            MouseArea {
                id: clickAreaForward

                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: {
                    newMessageLoader.selectedItem = {
                        "index": letterIndex,
                        "subject": letterTheme,
                        "sendTo": "",
                        "content": letterContent,
                        "IsForward": true,
                        "isDraft": false,
                        "isReply": false,
                        "title": "Forward",
                        "newTitle": "Forward"
                    };

                    newMessageLoader.source = "";
                    newMessageLoader.active = false;

                    newMessageLoader.active = true;
                    newMessageLoader.source = "screens/navigation/new_message/NewMessageQML.qml";
                }
            }
            Text {
                id: forward

                anchors.centerIn: parent
                color: "#4a5565"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                height: 20
                horizontalAlignment: Text.AlignHCenter
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                text: "Forward"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
                width: 52
            }
        }

        // Archive and Delete button
        Rectangle {
            id: container_8

            anchors.right: parent.right
            anchors.rightMargin: 14
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 5
            color: Color.transparent
            height: 28
            width: 58
            x: 568

            // Archive
            Rectangle {
                id: buttonToArchiveEmail

                anchors.left: parent.left
                color: Color.transparent
                height: 18
                radius: 10
                scale: clickAreaArchive.containsMouse ? 1.3 : 1.0
                width: 18

                Behavior on scale {
                    NumberAnimation {
                        duration: 150
                        easing.type: Easing.InOutQuad
                    }
                }

                MouseArea {
                    id: clickAreaArchive

                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true

                    onClicked: {
                        // MinimizeWindow
                    }
                }
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

            // Delete
            Rectangle {
                id: buttonToDeleteEmail

                anchors.right: parent.right
                color: Color.transparent
                height: 18
                radius: 10
                scale: clickAreaDelete.containsMouse ? 1.3 : 1.0
                width: 18

                Behavior on scale {
                    NumberAnimation {
                        duration: 150
                        easing.type: Easing.InOutQuad
                    }
                }

                MouseArea {
                    id: clickAreaDelete

                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true

                    onClicked: {
                        console.log("INDEX: " + letterIndex);
                        deleteClicked();
                    }
                }
                // TEMP ICON
                Image {
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    height: 18
                    source: "qrc:/pngs/assets/ic_bin.svg"
                    sourceSize.height: height * Screen.devicePixelRatio
                    sourceSize.width: width * Screen.devicePixelRatio
                    width: 18
                }
            }
        }
    }
}