import QtQuick
import QtQuick.Shapes

Rectangle {
    height: 92
    color: "#ffffff"

    property bool starred: false
    property string theme: "ThemeHolder"
    property string name: "NameHolder"
    property string preview: "TextHolderTextHolderTextHolderTextHolde...."
    property string time: "10:30 AM"

    //Bottom border
    Rectangle
    {
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

        // TEMP ICON
        Rectangle {
            id: button_margin_2
            height: 17
            width: 15
            anchors.verticalCenter: parent.verticalCenter
            color: "transparent"

            Rectangle {
                id: buttonToFavouriteHolder_1
                y: 2
                height: 15
                width: 15
                clip: true
                color: "transparent"

                Image {
                    anchors.centerIn: parent
                    width: 15
                    height: 15

                    source: "qrc:/pngs/assets/ic_star.svg"
                    fillMode: Image.PreserveAspectFit
                }
            }
        }

        // Content of item
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
                        id: nameHolder_1
                        height: 20
                        width: parent.width
                        color: "#101828"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Black
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                        text: name
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }

                Rectangle {
                    id: timeHolder
                    anchors.right: parent.right
                    y: 2
                    height: 16
                    width: 59
                    color: "transparent"

                    Text {
                        id: aM
                        x: 12
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
                    }
                }
            }

            // Theme of item
            Rectangle {
                id: themeHolder
                anchors.left: parent.left
                anchors.right: parent.right
                y: 22
                height: 20
                clip: true
                color: "transparent"

                Text {
                    id: themeHolder_1
                    height: 20
                    width: parent.width
                    color: "#101828"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Black
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    text: theme
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                }
            }

            // Text preview
            Rectangle {
                id: textHolder
                anchors.left: parent.left
                anchors.right: parent.right
                y: 44
                height: 16
                clip: true
                color: "transparent"

                Text {
                    id: textHolderText
                    height: 16
                    width: parent.width
                    color: "#6a7282"
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: preview
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                }
            }
        }
    }
}
