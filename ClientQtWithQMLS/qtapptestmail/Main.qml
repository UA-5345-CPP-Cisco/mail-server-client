import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    visible: true
    width: 1024
    height: 768
    minimumHeight: 500
    minimumWidth: 750
    title: "Mail Client Interface"

    RowLayout {
        anchors.fill: parent
        spacing: 0

        NavigationQML {
            id: navMenu

            Layout.preferredWidth: 250
            Layout.fillHeight: true
        }

        SplitView {
            id: splitView

            Layout.fillWidth: true
            Layout.fillHeight: true

            orientation: Qt.Horizontal

            handle:Rectangle
            {
                id: handleDelegate
                color: "transparent"
                containmentMask: Item
                {
                    x: (handleDelegate.width - width) / 2
                    width: 5
                    height: splitView.height
                }
            }

            EmailsListQML {
                SplitView.preferredWidth: 350
                SplitView.minimumWidth: 250
                SplitView.fillHeight: true
            }

            ContentBlankPageQML {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                SplitView.minimumWidth: 250
            }
        }
    }
}
