// AppToolTip.qml
import QtQuick
import QtQuick.Controls

ToolTip {
    id: root
    delay: 400

    contentItem: Text
    {
        text: root.text
        color: Color.secondaryText
        font.family: "Segoe UI"
        font.pixelSize: 12
    }

    background: Rectangle
    {
        color:Color.hover
        radius: 6
    }
}
