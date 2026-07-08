import QtQuick
import QtQuick.Effects
import QtQuick.Shapes
import QtQuick.Controls

Rectangle {
    id: root

    property string content: preview
    property string name: "NameHolder"
    property string preview: "TextHolderTextHolderTextHolderTextHolde...."
    property var searchModel: null
    property string sendTo: ""
    property bool starred: false
    property string theme: "ThemeHolder"
    property string time: "10:30"
    property int state: 0

    signal deleteClicked
    signal openRequested(string theme, string name, string sendTo, string content, string time, bool starred)
    signal starredClicked

    color: state === 2 ? Color.highlight : state === 1 ? Color.surface : Color.background
    height: 92

    Behavior on color {
        ColorAnimation {
            duration: 150
            easing.type: Easing.OutCubic
        }
    }

    MouseArea
    {
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true

        onClicked: mouse => {
            if (mouse.button === Qt.LeftButton) {
                root.state = containsMouse ? 1 : 0;
                var starPos = favouriteButtonContainer.mapToItem(root, 0, 0);
                var inStarZone = (mouse.x >= starPos.x - 8 && mouse.x <= starPos.x + favouriteButtonContainer.width + 8 && mouse.y >= starPos.y - 8 && mouse.y <= starPos.y + favouriteButtonContainer.height + 8);
                if (!inStarZone) {
                    root.openRequested(root.theme, root.name, root.sendTo, root.content, root.time, root.starred);
                }
            } else if (mouse.button === Qt.RightButton) {
                contextMenu.popup();
            }
        }
        onEntered: root.state = 2
        onExited: root.state = 0
        onPressed: mouse => {
            if (mouse.button === Qt.LeftButton) {
                root.state = 1;
            }
        }

    }
    Menu {
        id: contextMenu

        background: Rectangle {
            border.color: Color.outline
            border.width: 1
            color: Color.background
            implicitHeight: 40
            implicitWidth: 200
            radius: 8
        }
        delegate: MenuItem {
            id: menuItem

            implicitHeight: 36
            implicitWidth: 200
            padding: 0

            background: Rectangle {
                anchors.margins: 4
                color: menuItem.hovered ? Color.highlight : Color.transparent
                radius: 4
            }
            contentItem: Text {
                color: Color.secondaryText
                font.pixelSize: 13
                leftPadding: 12
                text: menuItem.text
                verticalAlignment: Text.AlignVCenter
            }
        }

        palette {
            base: Color.background
            highlight: Color.highlight
            highlightedText: Color.hover
            text: Color.secondaryText
        }
        MenuItem {
            text: "Copy"

            onTriggered: {}
        }
        MenuSeparator {
            padding: 4

            contentItem: Rectangle {
                color: Color.outline
                implicitHeight: 1
                implicitWidth: 200
            }
        }
        MenuItem {

            text: "Delete"
            onTriggered: root.deleteClicked()
        }
    }

    // Bottom border
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        color: Color.outline
        height: 1
    }
    Rectangle {
        id: itemContainer

        color: Color.transparent
        height: 60

        anchors {
            left: parent.left
            margins: 16
            right: parent.right
            top: parent.top
        }

        // Star icon
        Rectangle {
            id: favouriteButtonContainer

            anchors.verticalCenter: parent.verticalCenter
            color: Color.transparent
            height: 17
            scale: favouriteButtonClickArea.containsMouse ? 1.1 : 1.0
            width: 15

            Behavior on scale {
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            MouseArea {
                id: favouriteButtonClickArea

                anchors.fill: parent
                anchors.margins: -8
                cursorShape: Qt.PointingHandCursor
                z: 10

                onClicked: {
                    root.starred = !root.starred;
                    root.starredClicked();
                }
            }
            Rectangle {
                id: favouriteButton

                clip: true
                color: Color.transparent
                height: 18
                width: 18
                y: 2

                Image {
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    height: 18
                    source: starred ? "qrc:/pngs/assets/ic_star_active.svg" : "qrc:/pngs/assets/ic_star.svg"
                    sourceSize.height: height * Screen.devicePixelRatio
                    sourceSize.width: width * Screen.devicePixelRatio
                    width: 18
                }
            }
        }

        // Content
        Rectangle {
            id: contentContainer

            anchors.left: favouriteButtonContainer.right
            anchors.leftMargin: 8
            anchors.right: parent.right
            color: Color.transparent
            height: 60

            Rectangle {
                id: headerContainer

                anchors.left: parent.left
                anchors.right: parent.right
                clip: true
                color: Color.transparent
                height: 20

                Rectangle {
                    id: nameContainer

                    anchors.left: parent.left
                    anchors.right: timeContainer.left
                    clip: true
                    color: Color.transparent
                    height: 20

                    Text {
                        id: nameText

                        color: Color.primaryText
                        elide: Text.ElideRight
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Black
                        height: 20
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                        maximumLineCount: 1
                        text: searchModel ? searchModel.HighlightAllFoundWords(name, searchModel.SearchedText) : name
                        textFormat: Text.RichText
                        verticalAlignment: Text.AlignVCenter
                        width: parent.width
                        wrapMode: Text.NoWrap
                    }
                    HoverHandler {
                        id: nameHover
                    }
                    EmailToolTip {
                        text: name
                        visible: nameHover.hovered && (nameText.contentWidth > nameText.width)
                        x: 0
                        y: nameContainer.height + 4
                    }
                }
                Rectangle {
                    id: timeContainer

                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.verticalCenter: parent.verticalCenter
                    color: Color.transparent
                    height: 16
                    width: timeText.implicitWidth

                    Text {
                        id: timeText

                        anchors.fill: parent
                        color: Color.secondaryText
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignRight
                        text: time
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            Rectangle {
                id: themeContainer

                anchors.left: parent.left
                anchors.right: parent.right
                clip: true
                color: Color.transparent
                height: 20
                y: 22

                Text {
                    id: themeText

                    color: Color.primaryText
                    elide: Text.ElideRight
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Black
                    height: 20
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    maximumLineCount: 1
                    text: searchModel ? searchModel.HighlightAllFoundWords(theme, searchModel.SearchedText) : theme
                    textFormat: Text.RichText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    wrapMode: Text.NoWrap
                }
                HoverHandler {
                    id: themeHover
                }
                EmailToolTip {
                    text: theme
                    visible: themeHover.hovered && (themeText.contentWidth > themeText.width)
                    x: 0
                    y: themeContainer.height + 4
                }
            }
            Rectangle {
                id: previewContainer

                anchors.left: parent.left
                anchors.right: parent.right
                clip: true
                color: Color.transparent
                height: 16
                y: 44

                Text {
                    id: previewText

                    color: Color.secondaryText
                    elide: Text.ElideRight
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    height: 16
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    maximumLineCount: 1
                    text: searchModel ? searchModel.HighlightAllFoundWords(preview, searchModel.SearchedText) : preview
                    textFormat: Text.RichText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    wrapMode: Text.NoWrap
                }
                HoverHandler {
                    id: previewHover
                }
                EmailToolTip {
                    text: content
                    visible: previewHover.hovered
                    x: 0
                    y: previewContainer.height + 4
                }
            }
        }
    }
}