import QtQuick

Rectangle {
    id:root
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

    Behavior on color {
        ColorAnimation { duration: 150; easing.type: Easing.OutCubic }
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
                    height: 20; width: parent.width
                    color: "#101828"
                    font.family: "Segoe UI"; font.pixelSize: 14; font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20; lineHeightMode: Text.FixedHeight
                    text: searchModel ? searchModel.highlightAllFoundWords(name, searchModel.searchedText) : name
                    textFormat: Text.RichText; verticalAlignment: Text.AlignVCenter
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
                    font.family: "Segoe UI"; font.pixelSize: 12; font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16; lineHeightMode: Text.FixedHeight
                    text: time; textFormat: Text.PlainText
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
            font.family: "Segoe UI"; font.pixelSize: 12; font.weight: Font.Normal
            horizontalAlignment: Text.AlignLeft
            lineHeight: 16; lineHeightMode: Text.FixedHeight
            text: "Draft"; textFormat: Text.PlainText
            verticalAlignment: Text.AlignVCenter
        }
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered:  root.color = "#f9fafb"
        onExited:   root.color = "#ffffff"
        onPressed:  root.color = "#f3f4f6"
        onReleased: {
            root.color = containsMouse ? "#f9fafb" : "#ffffff"
            root.openRequested(root.theme, root.name, root.sendTo, root.content, root.time)
        }
        onCanceled: root.color = "#ffffff"
    }
}
