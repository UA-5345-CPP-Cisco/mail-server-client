import QtQuick
import QtQuick.Effects
import QtQuick.Shapes
import QtQuick.Controls

Rectangle
{
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

    Behavior on color
    {
        ColorAnimation
        {
            duration: 150
            easing.type: Easing.OutCubic
        }
    }
    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onEntered: root.color = "#f9fafb"
        onExited: root.color = "#ffffff"
        onPressed: root.color = "#f3f4f6"
        onClicked: mouse => {
            if (mouse.button === Qt.LeftButton)
            {
                root.color = containsMouse ? "#f9fafb" : "#ffffff"
                var starPos = favouriteButtonContainer.mapToItem(root, 0, 0)
                var inStarZone = (mouse.x >= starPos.x - 8
                    && mouse.x <= starPos.x + favouriteButtonContainer.width + 8
                    && mouse.y >= starPos.y - 8 && mouse.y
                    <= starPos.y + favouriteButtonContainer.height + 8)
                if (!inStarZone)
                {
                    root.openRequested(root.theme, root.name,
                        root.sendTo, root.content,
                        root.time, root.starred)
                }
            } else if (mouse.button === Qt.RightButton)
            {
                contextMenu.popup()
            }
        }
        onCanceled: root.color = "#ffffff"
    }
    Menu
    {
        id: contextMenu

        palette
        {
            base: "#ffffff"
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

        MenuItem
        {
            text: "Copy"
            onTriggered:
            {

            }
        }

        MenuSeparator
        {
            padding: 4
            contentItem: Rectangle
            {
                implicitWidth: 200
                implicitHeight: 1
                color: "#e5e7eb"
            }
        }

        MenuItem
        {
            text: "Delete"
            onClicked: root.deleteClicked()
        }
    }

    // Bottom border
    Rectangle
    {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 1
        color: "#e5e7eb"
    }

    Rectangle {
        id: itemContainer
        anchors
        {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 16
        }
        height: 60
        color: "transparent"

        // Star icon
        Rectangle
        {
            id: favouriteButtonContainer
            height: 17
            width: 15
            anchors.verticalCenter: parent.verticalCenter
            color: "transparent"
            MouseArea {
                id: favouriteButtonClickArea
                anchors.fill: parent
                anchors.margins: -8
                cursorShape: Qt.PointingHandCursor

                z: 10
                onClicked:
                {
                    root.starred = !root.starred
                    root.starredClicked()
                }
            }
            scale: favouriteButtonClickArea.containsMouse ? 1.1 : 1.0

            Behavior on scale
            {
                NumberAnimation
                {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            Rectangle
            {
                id: favouriteButton
                y: 2
                height: 18
                width: 18
                clip: true
                color: "transparent"

                Image
                {
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
        Rectangle
        {
            id: contentContainer
            anchors.left: favouriteButtonContainer.right
            anchors.leftMargin: 8
            anchors.right: parent.right
            height: 60
            color: "transparent"

            Rectangle
            {
                id: headerContainer
                anchors.left: parent.left
                anchors.right: parent.right
                height: 20
                clip: true
                color: "transparent"

                Rectangle
                {
                    id: nameContainer
                    anchors.left: parent.left
                    anchors.right: timeContainer.left
                    height: 20
                    clip: true
                    color: "transparent"

                    Text
                    {
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
                        text: searchModel ? searchModel.HighlightAllFoundWords(
                            name,
                            searchModel.SearchedText) : name
                        textFormat: Text.RichText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.NoWrap
                        elide: Text.ElideRight
                        maximumLineCount: 1
                    }

                    HoverHandler
                    {
                        id: nameHover
                    }

                    EmailToolTip
                    {
                        visible: nameHover.hovered && (nameText.contentWidth > nameText.width)
                        text: name
                        x: 0
                        y: nameContainer.height + 4
                    }
                }

                Rectangle
                {
                    id: timeContainer
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.verticalCenter: parent.verticalCenter
                    height: 16
                    width: timeText.implicitWidth
                    color: "transparent"

                    Text
                    {
                        id: timeText
                        anchors.fill: parent
                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: time
                        textFormat: Text.PlainText
                    }
                }
            }

            Rectangle
            {
                id: themeContainer
                anchors.left: parent.left
                anchors.right: parent.right
                y: 22
                height: 20
                clip: true
                color: "transparent"

                Text
                {
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
                    text: searchModel ? searchModel.HighlightAllFoundWords(
                        theme,
                        searchModel.SearchedText) : theme
                    textFormat: Text.RichText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                    maximumLineCount: 1
                }

                HoverHandler
                {
                    id: themeHover
                }

                EmailToolTip
                {
                    visible: themeHover.hovered
                        && (themeText.contentWidth > themeText.width)
                    text: theme
                    x: 0
                    y: themeContainer.height + 4
                }
            }

            Rectangle
            {
                id: previewContainer
                anchors.left: parent.left
                anchors.right: parent.right
                y: 44
                height: 16
                clip: true
                color: "transparent"

                Text
                {
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
                    text: searchModel ? searchModel.HighlightAllFoundWords(
                        preview,
                        searchModel.SearchedText) : preview
                    textFormat: Text.RichText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                    maximumLineCount: 1
                }

                HoverHandler
                {
                    id: previewHover
                }

                EmailToolTip
                {
                    visible: previewHover.hovered
                    text: content
                    x: 0
                    y: previewContainer.height + 4
                }
            }
        }
    }
}