import QtQuick
import QtQuick.Shapes

Rectangle {
    id: contentBlankPageQML

    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.top: parent.top
    clip: true
    color: "#ffffff"
    height: 400
    width: 400

    Rectangle {
        id: background

        anchors.centerIn: parent
        color: "#ffffff"

        Rectangle {
            id: container

            anchors.centerIn: parent
            color: "transparent"

            Rectangle {
                id: container_1

                anchors.centerIn: parent
                color: "transparent"
                height: 80
                width: 141

                Rectangle {
                    id: sVG

                    clip: true
                    color: "transparent"
                    height: 48
                    opacity: 0.30
                    width: 48
                    x: 46.50

                    Image {
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        height: 48
                        source: "qrc:/pngs/assets/ic_select_email.svg"
                        sourceSize.height: height * Screen.devicePixelRatio
                        sourceSize.width: width * Screen.devicePixelRatio
                        width: 48
                    }
                }
                Rectangle {
                    id: container_2

                    color: "transparent"
                    height: 20
                    width: 141
                    y: 60

                    Text {
                        id: select_an_email_to_read

                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        height: 20
                        horizontalAlignment: Text.AlignHCenter
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                        text: "Select an email to read"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                        width: 142
                    }
                }
            }
        }
    }
}