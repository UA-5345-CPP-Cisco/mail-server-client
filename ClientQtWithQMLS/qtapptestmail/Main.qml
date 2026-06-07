import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window
    visible: true
    width: 1024
    height: 768
    title: "Mail Client Interface"

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        NavigationQML {
            SplitView.preferredWidth: 200
            SplitView.minimumWidth: 150
            SplitView.maximumWidth: 300
        }

        EmailsListQML {
            SplitView.preferredWidth: 300
            SplitView.minimumWidth: 200
            SplitView.maximumWidth: 500
        }

        ContentBlankPageQML {
            SplitView.fillWidth: true
            SplitView.minimumWidth: 300
        }
    }
}