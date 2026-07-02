// AppToolTip.qml
import QtQuick
import QtQuick.Controls

ToolTip {
    id: root
    delay: 400

    contentItem: Text {
        text: root.text
        color: "#ffffff"
        font.family: "Segoe UI"
        font.pixelSize: 12
    }

    background: Rectangle {
        color: "#1f2937"
        radius: 6
    }
}
