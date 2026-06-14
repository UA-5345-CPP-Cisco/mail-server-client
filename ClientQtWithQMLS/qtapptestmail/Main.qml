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

    Component.onCompleted: {
        emailsModel.AddData(false, false, false, "no", "no", "no", "no", "no")

        emailsModel.AddData(true, false, false, "no", "no", "no", "no", "no")
        emailsModel.AddData(true, false, false, "no", "no", "no", "no", "no")
        emailsModel.AddData(true, false, false, "no", "no", "no", "no", "no")

        emailsModel.AddData(false, false, true, "no", "no", "no", "no", "no")
        emailsModel.AddData(false, false, true, "no", "no", "no", "no", "no")
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            window.contentItem.forceActiveFocus()
        }
    }

    function closeMessageWindow()
    {
        newMessageLoader.active = false
        newMessageLoader.source = ""
    }

    //new message loader
    Loader
    {
        id: newMessageLoader
        active: false
        z: 999
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        width: item ? item.implicitWidth : 0
        height: item ? item.implicitHeight : 0

        source: ""
        Behavior on opacity
        {
            NumberAnimation { duration: 200 }
        }

        opacity: status === Loader.Ready ? 1 : 0
    }


    RowLayout
    {
        anchors.fill: parent
        spacing: 0

        NavigationQML
        {
            id: navMenu

            Layout.preferredWidth: 250
            Layout.fillHeight: true

            onInboxClicked: {
                emailList.isDraftMode = false
                emailList.sourceModel = inboxModel
            }
            onStarredClicked: {
                emailList.isDraftMode = false
                emailList.sourceModel = starredModel
            }
            onSentClicked: {
                emailList.isDraftMode = false
                emailList.sourceModel = sentModel
            }
            onDraftClicked: {
                emailList.isDraftMode = true
                emailList.sourceModel = draftModel
            }
        }

        SplitView
        {
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

            EmailsListQML
            {
                id:emailList
                SplitView.preferredWidth: 350
                SplitView.minimumWidth: 250
                SplitView.fillHeight: true
            }

            ContentBlankPageQML
            {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                SplitView.minimumWidth: 250
            }
        }
    }
}
