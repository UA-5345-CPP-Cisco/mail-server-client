import QtQuick
import QtQuick.Controls

Rectangle
{
    id:root
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    height: 116
    color: Color.background

    property string theme: "Quarterly Report Summary"
    property string name: "ceo@company.com"
    property string preview: "Draft of the quarterly summary for leadership review..."
    property string content: "Draft of the quarterly summary for leadership review..."
    property string sendTo: ""
    property string time: "Jun 4"
    property var searchModel: null

    signal openRequested(string theme, string name, string sendTo, string content, string time)
    signal clicked()
    signal deleteClicked()

    Behavior on color
    {
        ColorAnimation { duration: 150; easing.type: Easing.OutCubic }
    }

    Rectangle
    {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 1
        color: Color.outline
    }

    Rectangle
    {
        id: itemContainer

        anchors
        {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 16
        }
        height: 84
        color: Color.transparent

        // Content
        Rectangle
        {
            id: headerContainer
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20
            color: Color.transparent

            // Name
            Rectangle
            {
                id: nameContainer
                anchors.left: parent.left
                anchors.right: timeContainer.left
                height: 20
                clip: true
                color: Color.transparent

                Text
                {
                    id: nameText
                    height: 20;
                    width: parent.width
                    color: Color.button
                    font.family: "Segoe UI";
                    font.pixelSize: 14; font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20;
                    lineHeightMode: Text.FixedHeight
                    text: searchModel ? searchModel.HighlightAllFoundWords(name, searchModel.SearchedText) : name
                    textFormat: Text.RichText;
                    verticalAlignment: Text.AlignVCenter
                }
            }

            // Time
            Rectangle
            {
                id: timeContainer
                anchors.right: parent.right
                y: 2; height: 16; width: 44
                color: Color.transparent

                Text
                {
                    id: timeText
                    x: 8; height: 16; width: 29
                    color: Color.secondaryText
                    font.family: "Segoe UI";
                    font.pixelSize: 12;
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16;
                    lineHeightMode: Text.FixedHeight
                    text: formatEmailTime(time);
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // Theme
        Rectangle
        {
            id: themeContainer
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: headerContainer.bottom
            anchors.topMargin: 2
            height: 20
            clip: true; color: Color.transparent

            Text
            {
                id: themeText
                height: 20; width: parent.width
                color: Color.button
                font.family: "Segoe UI"; font.pixelSize: 14; font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                lineHeight: 20; lineHeightMode: Text.FixedHeight
                text: searchModel ? searchModel.HighlightAllFoundWords(theme, searchModel.SearchedText) : theme
                textFormat: Text.RichText; verticalAlignment: Text.AlignVCenter
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                maximumLineCount: 1
            }
        }

        // Text preview
        Rectangle
        {
            id: previewContainer
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: themeContainer.bottom
            anchors.topMargin: 2
            height: 20
            clip: true; color: Color.transparent

            Text
            {
                id: previewText
                height: 16; width: parent.width
                color: Color.secondaryText
                font.family: "Segoe UI"; font.pixelSize: 12; font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                lineHeight: 16; lineHeightMode: Text.FixedHeight
                text: searchModel ? searchModel.HighlightAllFoundWords(preview, searchModel.SearchedText) : preview
                textFormat: Text.RichText; verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }
        }

        // Draft mark
        Text
        {
            id: draftBadgeText
            anchors.top: previewContainer.bottom
            anchors.topMargin: 2
            height: 16; width: 28
            color: "#fe9a00"
            font.family: "Segoe UI";
            font.pixelSize: 12; font.weight: Font.Normal
            horizontalAlignment: Text.AlignLeft
            lineHeight: 16;
            lineHeightMode: Text.FixedHeight
            text: "Draft";
            textFormat: Text.PlainText
            verticalAlignment: Text.AlignVCenter
        }
    }

    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onEntered: root.color = Color.surface
        onExited: root.color = Color.background

        onPressed: (mouse) =>
        {
            if (mouse.button === Qt.LeftButton)
            {
                root.color = Color.highlight
            }
        }

        onClicked: (mouse) =>
            {
            if (mouse.button === Qt.LeftButton)
            {
                root.clicked()
            }
            else if (mouse.button === Qt.RightButton)
            {
                contextMenu.x = mouse.x
                contextMenu.y = mouse.y
                contextMenu.popup()
            }
        }

        onReleased:
        {
            root.color = containsMouse ? Color.surface : Color.background
        }

        onCanceled: root.color = Color.background
    }
    Menu
    {
        id: contextMenu

        // The maximum allowed distance (in pixels) the cursor can wander away from the menu boundaries
        property real maxDistance: 40

        palette
        {
            base: Color.background
            text:Color.hover
            highlight: Color.highlight
            highlightedText:Color.hover
        }

        background: Rectangle
        {
            implicitWidth: 200
            implicitHeight: 40
            color: Color.background
            radius: 8
            border.color: Color.outline
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
                color: menuItem.hovered ?Color.hover : Color.outline
                font.pixelSize: 13
                leftPadding: 12
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle
            {
                color: menuItem.hovered ? Color.highlight : Color.transparent
                radius: 4
                anchors.margins: 4
            }
        }

        MenuItem
        {
            text: "Copy"
            onTriggered: { }
        }

        MenuSeparator
        {
            padding: 4
            contentItem: Rectangle {
                implicitWidth: 200
                implicitHeight: 1
                color: Color.outline
            }
        }

        MenuItem
        {
            text: "Delete"
            onClicked: root.deleteClicked()
        }


        MouseArea {
            id: tracker
            parent: Overlay.overlay
            anchors.fill: parent
            hoverEnabled: true
            enabled: false

            onPositionChanged: (mouse) =>
            {

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