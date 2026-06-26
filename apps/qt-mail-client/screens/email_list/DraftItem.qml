import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    height: 116
    color: "#ffffff"

    property string theme: "Quarterly Report Summary"
    property string name: "ceo@company.com"
    property string preview: "Draft of the quarterly summary for leadership review..."
    property string content: preview
    property string sendTo: ""
    property string time: "Jun 4"
    property var searchModel: null

    signal openRequested(string theme, string name, string sendTo, string content, string time)

    signal clicked()

    signal deleteClicked()

    Behavior on color {
        ColorAnimation {
            duration: 150; easing.type: Easing.OutCubic
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 1
        color: "#e5e7eb"
    }

    Rectangle {
        id: container_3

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 16
        }
        height: 84
        color: "transparent"

        // Content
        Rectangle {
            id: container_4
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20
            color: "transparent"

            // Name
            Rectangle {
                id: container_5
                anchors.left: parent.left
                anchors.right: dateHolder.left
                height: 20
                clip: true
                color: "transparent"

                Text {
                    id: ceo_company_com
                    height: 20;
                    width: parent.width
                    color: "#101828"
                    font.family: "Segoe UI";
                    font.pixelSize: 14; font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20;
                    lineHeightMode: Text.FixedHeight
                    text: searchModel ? searchModel.highlightAllFoundWords(name, searchModel.searchedText) : name
                    textFormat: Text.RichText;
                    verticalAlignment: Text.AlignVCenter
                }
            }

            // Time
            Rectangle {
                id: dateHolder
                anchors.right: parent.right
                y: 2; height: 16; width: 44
                color: "transparent"

                Text {
                    id: jun_4
                    x: 8; height: 16; width: 29
                    color: "#6a7282"
                    font.family: "Segoe UI";
                    font.pixelSize: 12;
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16;
                    lineHeightMode: Text.FixedHeight
                    text: format_email_time(time);
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // Theme
        Rectangle {
            id: container_6
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: container_4.bottom
            anchors.topMargin: 2
            height: 20
            clip: true; color: "transparent"

            Text {
                id: quarterly_Report_Summary
                height: 20; width: parent.width
                color: "#101828"
                font.family: "Segoe UI"; font.pixelSize: 14; font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                lineHeight: 20; lineHeightMode: Text.FixedHeight
                text: searchModel ? searchModel.highlightAllFoundWords(theme, searchModel.searchedText) : theme
                textFormat: Text.RichText; verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }
        }

        // Text preview
        Rectangle {
            id: container_7
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: container_6.bottom
            anchors.topMargin: 2
            height: 20
            clip: true; color: "transparent"

            Text {
                id: draft_preview
                height: 16; width: parent.width
                color: "#6a7282"
                font.family: "Segoe UI"; font.pixelSize: 12; font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                lineHeight: 16; lineHeightMode: Text.FixedHeight
                text: searchModel ? searchModel.highlightAllFoundWords(preview, searchModel.searchedText) : preview
                textFormat: Text.RichText; verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }
        }

        // Draft mark
        Text {
            id: draftBadge
            anchors.top: container_7.bottom
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
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onEntered: root.color = "#f9fafb"
        onExited: root.color = "#ffffff"

        onPressed: (mouse) => {
            if (mouse.button === Qt.LeftButton) {
                root.color = "#f3f4f6"
            }
        }

        onClicked: (mouse) => {
            if (mouse.button === Qt.LeftButton) {
                root.clicked()
            } else if (mouse.button === Qt.RightButton) {
                contextMenu.x = mouse.x
                contextMenu.y = mouse.y
                contextMenu.popup()
            }
        }

        onReleased: {
            root.color = containsMouse ? "#f9fafb" : "#ffffff"
        }

        onCanceled: root.color = "#ffffff"
    }
    Menu {
        id: contextMenu

        // The maximum allowed distance (in pixels) the cursor can wander away from the menu boundaries
        property real maxDistance: 40

        palette {
            base: "#ffffff"
            text: "#1f2937"
            highlight: "#f3f4f6"
            highlightedText: "#1f2937"
        }

        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 40
            color: "#ffffff"
            radius: 8
            border.color: "#e5e7eb"
            border.width: 1
        }

        delegate: MenuItem {
            id: menuItem
            implicitWidth: 200
            implicitHeight: 36
            padding: 0

            contentItem: Text {
                text: menuItem.text
                color: menuItem.hovered ? "#1f2937" : "#6b7280"
                font.pixelSize: 13
                leftPadding: 12
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
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
                    mouse.y > p.y + contextMenu.height + 40) {
                    contextMenu.close()
                }
            }
        }
    }
}
