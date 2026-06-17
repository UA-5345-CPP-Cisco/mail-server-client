import QtQuick
import QtQuick.Shapes

Rectangle {
    id: root
    height: 92

    property bool starred: false
    property string theme: "ThemeHolder"
    property string name: "NameHolder"
    property string preview: "TextHolderTextHolderTextHolderTextHolde...."
    property string content: preview
    property string sendTo: ""
    property string time: "10:30 AM"
    property var searchModel: null

    signal openRequested(string theme, string name, string sendTo, string content, string time, bool starred)
    signal starredClicked()

    color: "#ffffff"

    Behavior on color {
        ColorAnimation { duration: 150; easing.type: Easing.OutCubic }
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
        MouseArea
        {
            id: clickAreaFavourite
            anchors.fill: parent

            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onClicked:
            {
                // MinimizeWindow
            }
        }
        scale: clickAreaFavourite.containsMouse ? 1.1 : 1.0

        Behavior on scale
        {
            NumberAnimation
            {
                duration: 150
                easing.type: Easing.InOutQuad
            }
        }
        // Star icon
        Rectangle {
            id: button_margin_2
            height: 17
            width: 15
            anchors.verticalCenter: parent.verticalCenter
            color: "transparent"


            Rectangle {
                id: buttonToFavouriteHolder_1
                y: 2
                height: 18
                width: 18
                clip: true
                color: "transparent"

                Image
                {
                   source: starred ? "qrc:/pngs/assets/ic_plus.svg" : "qrc:/pngs/assets/ic_star.svg"
                   width: 18
                   height: 18
                   sourceSize.width: width * Screen.devicePixelRatio
                   sourceSize.height: height * Screen.devicePixelRatio
                   fillMode: Image.PreserveAspectFit
                   anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    anchors.margins: -8
                    z: 10
                    onClicked: {
                        root.starred = !root.starred
                        root.starredClicked()
                    }
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
                color: "transparent"

                Rectangle {
                    id: nameHolder
                    anchors.left: parent.left
                    anchors.right: timeHolder.left
                    height: 20
                    clip: true
                    color: "transparent"

                    Text {
                        height: 20; width: parent.width
                        color: "#101828"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Black
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 20; lineHeightMode: Text.FixedHeight
                        text: searchModel ? searchModel.highlightAllFoundWords(name, searchModel.searchedText) : name
                        textFormat: Text.RichText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }

                Rectangle {
                    id: timeHolder
                    anchors.right: parent.right
                    y: 2; height: 16; width: 59
                    x: 30
                    anchors.rightMargin: -8
                    color: "transparent"

                    Text {
                        x: 12
                        height: 16
                        width: 52
                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 16; lineHeightMode: Text.FixedHeight
                        text: time
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            Rectangle {
                id: themeHolder
                anchors.left: parent.left
                anchors.right: parent.right
                y: 22; height: 20; clip: true
                color: "transparent"

                Text {
                    height: 20; width: parent.width
                    color: "#101828"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Black
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20; lineHeightMode: Text.FixedHeight
                    text: searchModel ? searchModel.highlightAllFoundWords(theme, searchModel.searchedText) : theme
                    textFormat: Text.RichText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                }
            }

            Rectangle {
                id: textHolder
                anchors.left: parent.left
                anchors.right: parent.right
                y: 44; height: 16; clip: true
                color: "transparent"

                Text {
                    height: 16; width: parent.width
                    color: "#6a7282"
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16; lineHeightMode: Text.FixedHeight
                    text: searchModel ? searchModel.highlightAllFoundWords(preview, searchModel.searchedText) : preview
                    textFormat: Text.RichText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true

        onEntered:  root.color = "#f9fafb"
        onExited:   root.color = "#ffffff"
        onPressed:  root.color = "#f3f4f6"
        onReleased: (mouse) => {
            root.color = containsMouse ? "#f9fafb" : "#ffffff"
            var starPos = button_margin_2.mapToItem(root, 0, 0)
            var inStarZone = (mouse.x >= starPos.x - 8 && mouse.x <= starPos.x + button_margin_2.width + 8 &&
                            mouse.y >= starPos.y - 8 && mouse.y <= starPos.y + button_margin_2.height + 8)
            if (!inStarZone) {
                root.openRequested(root.theme, root.name, root.sendTo, root.content, root.time, root.starred)
            }
        }
        onCanceled: root.color = "#ffffff"
    }
}
