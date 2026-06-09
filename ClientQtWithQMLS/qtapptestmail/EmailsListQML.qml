import QtQuick
import QtQuick.Shapes

Rectangle {
    id: emailsListQML
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

    // Left border
    Rectangle {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 1
        color: "#e5e7eb"
        z:1
    }


    // Search section
    Rectangle {
        id: searchSection

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: 70
        color: "#ffffff"

        Rectangle {
            id: searchLine

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: 16
            }

            height: 38
            color: "transparent"

            // Search line
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

                // TEMP SEARCH ICON
                Item {
                    id: searchIconArea
                    x: 12
                    y: 11
                    width: 16
                    height: 16

                    Shape {
                        id: _vector
                        x: 2; y: 2
                        width: 10.67; height: 10.67

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
                        x: 11.13; y: 11.13
                        width: 2.87; height: 2.87

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

                // PlaceHolder
                Rectangle {
                    id: container
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

    // Inbox Separator
    Rectangle {
        id: separator

        anchors.top:searchSection.bottom
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.right: parent.right

        height: 34
        color: "#f9fafb"

        // Top border
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 1
            color: "#e5e7eb"
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
            id: heading_3
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: 16
            }
            height: 18
            color: "transparent"

            // Inbox and page label
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
                    id: inbox
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
                    text: "Inbox"
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

            // Navigation buttons
            Item {
                id: navButtons
                x: 564
                y: -8
                anchors.right: parent.right
                anchors.rightMargin: -6
                width: 50
                height: 18

                // Button back
                Rectangle {
                    id: button_margin
                    anchors.left: parent.left
                    width: 15
                    height: 17
                    color: "transparent"

                    Rectangle {
                        id: buttonToMoveBack
                        y: 2
                        height: 15
                        width: 15
                        clip: true
                        color: "transparent"

                        //TEMP BACK ICON
                        Shape {
                            id: iconToMoveBack
                            x: 4; y: 1
                            height: 12; width: 6
                            rotation: -180

                            ShapePath {
                                fillColor: "#00000000"
                                strokeColor: "#6a7282"
                                strokeWidth: 2
                                PathSvg { path: "M 0 12 L 6 6 L 0 0" }
                            }
                        }
                    }
                }

                // Button forward
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

                        //TEMP FORWARD ICON
                        Shape {
                            id: iconToMoveForward
                            x: 5; y: 1
                            height: 12; width: 6

                            ShapePath {
                                fillColor: "#00000000"
                                strokeColor: "#6a7282"
                                strokeWidth: 2
                                PathSvg { path: "M 0 12 L 6 6 L 0 0" }
                            }
                        }
                    }
                }
            }
        }
    }

  ListItem{
      id:itemList
      theme: "Sea"
      name: "Fish"
      preview: "Something..."
      time: "10:20pm"
      anchors.top:separator.bottom
      anchors.leftMargin: 0
      anchors.rightMargin: 0
      anchors.topMargin: 0
  }

  ListItem{
      id:itemList1
      anchors.top:itemList.bottom
      anchors.leftMargin: 0
      anchors.rightMargin: 0
      anchors.topMargin: 0
  }

  ListItem{
      id:itemList2
      anchors.top:itemList1.bottom
      anchors.leftMargin: 0
      anchors.rightMargin: 0
      anchors.topMargin: 0
  }
}
