import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    property string content: "Draft of the quarterly summary for leadership review..."
    property string name: "ceo@company.com"
    property string preview: "Draft of the quarterly summary for leadership review..."
    property var searchModel: null
    property string sendTo: ""
    property string theme: "Quarterly Report Summary"
    property string time: "Jun 4"
    property int state: 0

    signal clicked
    signal deleteClicked
    signal openRequested(string theme, string name, string sendTo, string content, string time)

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    color: state === 2 ? Color.highlight : state === 1 ? Color.surface : Color.background
    height: 116

    Behavior on color {
        ColorAnimation {
            duration: 150
            easing.type: Easing.OutCubic
        }
    }

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
        height: 84

        anchors {
            left: parent.left
            margins: 16
            right: parent.right
            top: parent.top
        }

        // Content
        Rectangle {
            id: headerContainer

            anchors.left: parent.left
            anchors.right: parent.right
            color: Color.transparent
            height: 20

            // Name
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
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    height: 20
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    text: searchModel ? searchModel.HighlightAllFoundWords(theme, searchModel.SearchedText) : theme
                    textFormat: Text.RichText
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                }
            }

            // Time
            Rectangle {
                id: timeContainer

                anchors.right: parent.right
                color: Color.transparent
                height: 16
                width: 44
                y: 2

                Text {
                    id: timeText

                    color: Color.secondaryText
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    height: 16
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: formatEmailTime(time)
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    width: 29
                    x: 8
                }
            }
        }

        // Theme
        Rectangle {
            id: themeContainer

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: headerContainer.bottom
            anchors.topMargin: 2
            clip: true
            color: Color.transparent
            height: 20

            Text {
                id: themeText

                color: Color.primaryText
                elide: Text.ElideRight
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
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
        }

        // Text preview
        Rectangle {
            id: previewContainer

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: themeContainer.bottom
            anchors.topMargin: 2
            clip: true
            color: Color.transparent
            height: 20

            Text {
                id: previewText

                color: Color.secondaryText
                font.family: "Segoe UI"
                font.pixelSize: 12
                font.weight: Font.Normal
                height: 16
                horizontalAlignment: Text.AlignLeft
                lineHeight: 16
                lineHeightMode: Text.FixedHeight
                text: searchModel ? searchModel.HighlightAllFoundWords(preview, searchModel.SearchedText) : preview
                textFormat: Text.RichText
                verticalAlignment: Text.AlignVCenter
                width: parent.width
                wrapMode: Text.Wrap
            }
        }

        // Draft mark
        Text {
            id: draftBadgeText

            anchors.top: previewContainer.bottom
            anchors.topMargin: 2
            color: "#fe9a00"
            font.family: "Segoe UI"
            font.pixelSize: 12
            font.weight: Font.Normal
            height: 16
            horizontalAlignment: Text.AlignLeft
            lineHeight: 16
            lineHeightMode: Text.FixedHeight
            text: "Draft"
            textFormat: Text.PlainText
            verticalAlignment: Text.AlignVCenter
            width: 28
        }
    }
    MouseArea {
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        anchors.fill: parent
        hoverEnabled: true

        onClicked: mouse => {
            if (mouse.button === Qt.LeftButton) {
                root.clicked();
            } else if (mouse.button === Qt.RightButton) {
                contextMenu.x = mouse.x;
                contextMenu.y = mouse.y;
                contextMenu.popup();
            }
        }
        onEntered: root.state = 2
        onExited: root.state = 0
        onPressed: mouse => {
            if (mouse.button === Qt.LeftButton) {
                root.state = 1
            }
        }
        onReleased: {
            root.state = containsMouse ? 1 : 0;
        }
    }
    Menu {
        id: contextMenu

        // The maximum allowed distance (in pixels) the cursor can wander away from the menu boundaries
        property real maxDistance: 40

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
        MouseArea {
            id: tracker

            anchors.fill: parent
            enabled: false
            hoverEnabled: true
            parent: Overlay.overlay

            onPositionChanged: mouse => {
                var p = contextMenu.mapToItem(parent, 0, 0);

                if (mouse.x < p.x - 40 || mouse.x > p.x + contextMenu.width + 40 || mouse.y < p.y - 40 || mouse.y > p.y + contextMenu.height + 40) {
                    contextMenu.close();
                }
            }
        }
    }
}
