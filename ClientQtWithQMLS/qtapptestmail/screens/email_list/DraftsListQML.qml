import QtQuick
import QtQuick.Shapes

Rectangle {
    id: draftsListQML

    color: "#ffffff"

    // Right border (handle of SplitView)
    Rectangle {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 1
        color: "#e5e7eb"
        z: 1
    }

    // Search section
    Rectangle {
        id: horizontalBorder

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: 70
        color: "#ffffff"

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 1
            color: "#e5e7eb"
        }

        Rectangle {
            id: container

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: 16
            }

            height: 38
            color: "transparent"

            Rectangle {
                id: input

                anchors.left: parent.left
                anchors.right: parent.right
                height: 38

                border.color: "#d1d5dc"
                border.width: 1
                clip: true
                color: "#ffffff"
                radius: 10

                // TEMP ICON
                Item {
                    x: 12
                    y: 11
                    width: 16
                    height: 16

                    Shape {
                        id: _vector
                        x: 2
                        y: 2
                        height: 10.67
                        width: 10.67

                        ShapePath {
                            fillColor: "#00000000"
                            strokeColor: "#6a7282"
                            strokeWidth: 1.33
                            PathSvg {
                                path: "M 10.666666984558105 5.333333492279053 C 10.666666984558105 8.278852391606137 8.278852391606137 10.666666984558105 5.333333492279053 10.666666984558105 C 2.387814592951969 10.666666984558105 0 8.278852391606137 0 5.333333492279053 C 0 2.387814592951969 2.387814592951969 0 5.333333492279053 0 C 8.278852391606137 0 10.666666984558105 2.387814592951969 10.666666984558105 5.333333492279053 Z"
                            }
                        }
                    }
                    Shape {
                        id: _vector_1
                        x: 11.13
                        y: 11.13
                        height: 2.87
                        width: 2.87

                        ShapePath {
                            fillColor: "#00000000"
                            strokeColor: "#6a7282"
                            strokeWidth: 1.33
                            PathSvg {
                                path: "M 2.866666316986084 2.866666316986084 L 0 0"
                            }
                        }
                    }
                }

                Rectangle {
                    id: container_1
                    x: 36.80
                    y: 9.60
                    height: 19
                    width: parent.width - 36.80 - 8
                    clip: true
                    color: "transparent"

                    Text {
                        id: search_mail
                        height: 19
                        width: parent.width
                        color: "#99a1af"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        text: "Search mail"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }
            }
        }
    }

    // Drafts separator
    Rectangle {
        id: draftsSeparator

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: horizontalBorder.bottom

        height: 34
        color: "#f9fafb"

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 1
            color: "#e5e7eb"
        }

        Rectangle {
            id: heading_3

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: 16
            }
            height: 18
            color: "transparent"

            Rectangle {
                id: frame_3
                y: -8
                height: 18
                anchors.left: parent.left
                anchors.right: navButtons.left
                anchors.leftMargin: 0
                anchors.rightMargin: 0
                color: "transparent"

                Text {
                    id: draftsLabel
                    height: 16
                    color: "#6a7282"
                    font.capitalization: Font.AllUppercase
                    font.family: "Segoe UI"
                    font.letterSpacing: 0.60
                    font.pixelSize: 12
                    font.weight: Font.Black
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: "Drafts"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    id: amount
                    anchors.right: parent.right
                    y: 1
                    height: 16
                    color: "#000000"
                    font.capitalization: Font.AllUppercase
                    font.family: "Segoe UI"
                    font.letterSpacing: 0.60
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: "Amount"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Item {
                id: navButtons
                x: 564
                y: -8
                anchors.right: parent.right
                anchors.rightMargin: -6
                width: 50
                height: 18

                Rectangle {
                    id: button_margin
                    anchors.left: parent.left
                    width: 15
                    height: 17
                    color: "transparent"

                    Rectangle {
                        id: buttonToFavouriteHolder
                        y: 2
                        height: 15
                        width: 15
                        clip: true
                        color: "transparent"

                        // TEMP ICON
                        Shape {
                            id: buttonToMoveBack
                            x: 4
                            y: 1
                            height: 12
                            width: 6
                            rotation: -180

                            ShapePath {
                                fillColor: "#00000000"
                                strokeColor: "#6a7282"
                                strokeWidth: 2
                                PathSvg {
                                    path: "M 0 12 L 6 6 L 0 0"
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    id: button_margin_1
                    anchors.right: parent.right
                    width: 15
                    height: 18
                    color: "transparent"

                    Rectangle {
                        id: buttonToMoveForward
                        y: 2
                        height: 15
                        width: 15
                        clip: true
                        color: "transparent"

                        // TEMP ICON
                        Shape {
                            id: buttonToMoveForvard
                            x: 5
                            y: 2
                            height: 12
                            width: 6

                            ShapePath {
                                fillColor: "#00000000"
                                strokeColor: "#6a7282"
                                strokeWidth: 2
                                PathSvg {
                                    path: "M 0 12 L 6 6 L 0 0"
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // List of drafts
    Rectangle {
        id: container_2

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: draftsSeparator.bottom
        anchors.bottom: parent.bottom

        clip: true
        color: "transparent"

        DraftItem {
            id: item1
        }
        DraftItem {
            id: item2
            anchors.top: item1.bottom
        }
        DraftItem {
            id: item3
            anchors.top: item2.bottom
        }
        DraftItem {
            id: item4
            anchors.top: item3.bottom
        }
    }
}
