import QtQuick
import QtQuick.Layouts

Rectangle {
    color: "#ffffff"

    ColumnLayout {
        anchors.fill: parent
        DraftItem {
            Layout.fillWidth: true
            title: "No saved drafts"
            preview: "Draft persistence can be added on top of the compose form."
        }
    }
}
