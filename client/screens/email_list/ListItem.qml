import QtQuick
import QtQuick.Effects
import QtQuick.Shapes
import QtQuick.Controls

Rectangle {
    id: root
    height: 92

    property bool starred: false
    property string theme: "ThemeHolder"
    property string name: "NameHolder"
    property string preview: "TextHolderTextHolderTextHolderTextHolde...."
    property string content: preview
    property string sendTo: ""
    property string time: "10:30"
    property var searchModel: null

    signal openRequested(string theme, string name, string sendTo, string content, string time, bool starred)
    signal starredClicked
    signal deleteClicked

    color: "#ffffff"

    Behavior on color {
        ColorAnimation {
            duration: 150
            easing.type: Easing.OutCubic
        }
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onEntered: root.color = "#f9fafb"
        onExited: root.color = "#ffffff"
        onPressed: root.color = "#f3f4f6"
        onClicked: mouse => {
                       if (mouse.button === Qt.LeftButton) {
                           root.color = containsMouse ? "#f9fafb" : "#ffffff"
                           var starPos = button_margin_2.mapToItem(root, 0, 0)
                           var inStarZone = (mouse.x >= starPos.x - 8
                                             && mouse.x <= starPos.x + button_margin_2.width + 8
                                             && mouse.y >= starPos.y - 8 && mouse.y
                                             <= starPos.y + button_margin_2.height + 8)
                           if (!inStarZone) {
                               root.openRequested(root.theme, root.name,
                                                  root.sendTo, root.content,
                                                  root.time, root.starred)
                           }
                       } else if (mouse.button === Qt.RightButton) {
                           contextMenu.popup()
                       }
                   }
        onCanceled: root.color = "#ffffff"
    }
            onTriggered: {

            }
    // Bottom border
    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 1
        color: "#e5e7eb"
    }

    Rectangle {
        id: container_1
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 16
        }
        height: 60
        color: "transparent"

        // Star icon
        Rectangle {
            id: button_margin_2
            height: 17
            width: 15
            anchors.verticalCenter: parent.verticalCenter
            color: "transparent"
            MouseArea {
                id: clickAreaFavourite
                anchors.fill: parent
                anchors.margins: -8
                cursorShape: Qt.PointingHandCursor

                z: 10
                onClicked: {
                    root.starred = !root.starred
                    root.starredClicked()
                }
            }
            scale: clickAreaFavourite.containsMouse ? 1.1 : 1.0

            Behavior on scale {
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            Rectangle {
                id: buttonToFavouriteHolder_1
                y: 2
                height: 18
                width: 18
                clip: true
                color: "transparent"

                Image {
                    source: starred ? "qrc:/pngs/assets/ic_star_active.svg" : "qrc:/pngs/assets/ic_star.svg"
                    width: 18
                    height: 18
                    sourceSize.width: width * Screen.devicePixelRatio
                    sourceSize.height: height * Screen.devicePixelRatio
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }
        }

        // Content
        Rectangle {
            id: container_2
            anchors.left: button_margin_2.right
            anchors.leftMargin: 8
            anchors.right: parent.right
            height: 60
            color: "transparent"

            Rectangle {
                id: container_3
                anchors.left: parent.left
                anchors.right: parent.right
                height: 20
                clip: true
                color: "transparent"

                Rectangle {
                    id: nameHolder
                    anchors.left: parent.left
                    anchors.right: timeHolder.left
                    height: 20
                    clip: true
                    color: "transparent"

                    Text {
                        id: nameText
                        height: 20
                        width: parent.width
                        color: "#101828"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Black
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                        text: searchModel ? searchModel.highlightAllFoundWords(
                                                name,
                                                searchModel.searchedText) : name
                        textFormat: Text.RichText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.NoWrap
                        elide: Text.ElideRight
                        maximumLineCount: 1
                    }

                    HoverHandler {
                        id: nameHover
                    }

                    EmailToolTip {
                        visible: nameHover.hovered && (nameText.contentWidth > nameText.width)
                        text: name
                        x: 0
                        y: nameHolder.height + 4
                    }
                }

                Rectangle {
                    id: timeHolder
                    anchors.right: parent.right
                    y: 2
                    height: 16
                    width: 59
                    x: 30
                    anchors.rightMargin: -8
                    color: "transparent"
                    clip: true

                    Text {
                        id: timeText
                        x: 15
                        height: 16
                        width: 52
                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 16
                        lineHeightMode: Text.FixedHeight
                        text: time
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.NoWrap
                        elide: Text.ElideRight
                        maximumLineCount: 1
                    }
                }
            }

            Rectangle {
                id: themeHolder
                anchors.left: parent.left
                anchors.right: parent.right
                y: 22
                height: 20
                clip: true
                color: "transparent"

                Text {
                    id: themeText
                    height: 20
                    width: parent.width
                    color: "#101828"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Black
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    text: searchModel ? searchModel.highlightAllFoundWords(
                                            theme,
                                            searchModel.searchedText) : theme
                    textFormat: Text.RichText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                    maximumLineCount: 1
                }

                HoverHandler {
                    id: themeHover
                }

                EmailToolTip {
                    visible: themeHover.hovered
                             && (themeText.contentWidth > themeText.width)
                    text: theme
                    x: 0
                    y: themeHolder.height + 4
                }
            }

            Rectangle {
                id: previewHolder
                anchors.left: parent.left
                anchors.right: parent.right
                y: 44
                height: 16
                clip: true
                color: "transparent"

                Text {
                    id: previewText
                    height: 16
                    width: parent.width
                    color: "#6a7282"
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: searchModel ? searchModel.highlightAllFoundWords(
                                            preview,
                                            searchModel.searchedText) : preview
                    textFormat: Text.RichText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                    maximumLineCount: 1
                }

                HoverHandler {
                    id: previewHover
                }

                EmailToolTip {
                    visible: previewHover.hovered
                             && (previewText.contentWidth > previewText.width)
                    text: preview
                    x: 0
                    y: previewHolder.height + 4
            text: "#1f2937"
            highlight: "#f3f4f6"
            highlightedText: "#1f2937"
        }

        background: Rectangle
        {
            implicitWidth: 200
            implicitHeight: 40
            color: "#ffffff"
            radius: 8
            border.color: "#e5e7eb"
            border.width: 1
        }

        delegate: MenuItem
        {
            id: menuItem
            implicitWidth: 200
            implicitHeight: 36
            padding: 0

            contentItem: Text
            {
                text: menuItem.text
                color: menuItem.hovered ? "#1f2937" : "#6b7280"
                font.pixelSize: 13
                leftPadding: 12
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle
            {
                color: menuItem.hovered ? "#f3f4f6" : "transparent"
                radius: 4
                anchors.margins: 4
            }
        }

        MenuItem {
            text: "Copy"
            onTriggered: { }
        }

        MenuSeparator {
            padding: 4
            contentItem: Rectangle {
                implicitWidth: 200
                implicitHeight: 1
                color: "#e5e7eb"
            }
        }

        MenuItem {
            text: "Delete"
            onClicked: root.deleteClicked()
        }
        MouseArea {
            id: tracker
            parent: Overlay.overlay
            anchors.fill: parent
            hoverEnabled: true
            enabled: false

            onPositionChanged: (mouse) => {
                var p = contextMenu.mapToItem(parent, 0, 0)

                if (mouse.x < p.x - 40 ||
                    mouse.x > p.x + contextMenu.width + 40 ||
                    mouse.y < p.y - 40 ||
                    mouse.y > p.y + contextMenu.height + 40)
                {
                    contextMenu.close()
                }
            }
        }
    }
}
