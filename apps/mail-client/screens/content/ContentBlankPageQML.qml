import QtQuick
import QtQuick.Shapes

Rectangle {
    id: contentBlankPageQML
    anchors.top:parent.top
    anchors.right:parent.right
    anchors.bottom:parent.bottom
    width:400
    height:400
    clip: true
    color: Color.color("background")

    Rectangle {
        id: background

        color: Color.color("background")
        anchors.centerIn: parent

        Rectangle {
            id: container


            color: "transparent"
            anchors.centerIn: parent

            Rectangle {
                id: container_1


                height: 80
                width: 141

                color: "transparent"
                anchors.centerIn: parent

                 // TEMP ICON
                Rectangle {
                    id: sVG

                    x: 46.50

                    height: 48
                    width: 48

                    clip: true
                    color: "transparent"
                    opacity: 0.30

                    Image
                    {
                        source: "qrc:/pngs/assets/ic_select_email.svg"
                        width: 48
                        height: 48
                        sourceSize.width: width * Screen.devicePixelRatio
                        sourceSize.height: height * Screen.devicePixelRatio
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                    }
                }
                Rectangle
                {
                    id: container_2

                    y: 60

                    height: 20
                    width: 141

                    color: "transparent"

                    Text
                    {
                        id: select_an_email_to_read

                        height: 20
                        width: 142

                        color: Color.color("secondaryText")
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignHCenter
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                        text: "Select an email to read"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}
