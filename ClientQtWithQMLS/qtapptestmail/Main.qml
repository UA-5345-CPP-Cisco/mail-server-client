import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

ApplicationWindow {
    //properties
    property string selectedFolder: "inbox"
    property int inboxCount: inboxModel.totalEmailsCount
    property int starredCount: starredModel.totalEmailsCount
    property int sentCount: sentModel.totalEmailsCount
    property int draftsCount: draftModel.totalEmailsCount
    property string amountText: emailList.sourceModel ? emailList.sourceModel.pageAmountText : "0-0 of 0"

    //FunctionSorter
    function closeMessageWindow()
    {
        newMessageLoader.active = false
        newMessageLoader.source = ""

    }

    function closeSettingsWindow()
    {
        settingsLoader.active = false
        settingsLoader.source = ""
    }


    //blocks
    id: window
    visible: true
    width: 1024
    height: 768
    minimumHeight: 500
    minimumWidth: 750
    title: "Mail Client Interface"

    Component.onCompleted:
    {
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

        SplitView
        {
            id: splitView
            anchors.fill: parent
            orientation: Qt.Horizontal

            handle: Rectangle
            {
                id: handleDelegate
                color: "transparent"
                containmentMask: Item
                {
                    x: (handleDelegate.width - width) / 2
                    width: 5
                    height: splitView.height
                    MouseArea
                    {
                        anchors.fill: parent

                        hoverEnabled: true

                        cursorShape: Qt.SizeHorCursor

                        acceptedButtons: Qt.NoButton
                    }
                }
            }

            NavigationQML
            {
                id: navMenu

                Layout.preferredWidth: 250
                Layout.fillHeight: true
                SplitView.preferredWidth: 250
                SplitView.minimumWidth: 180
                SplitView.maximumWidth: 350
                SplitView.fillHeight: true

                onInboxClicked:
                {
                    emailList.isDraftMode = false
                    emailList.sourceModel = inboxModel
                    selectedFolder = "inbox"
                }
                onStarredClicked:
                {
                    emailList.isDraftMode = false
                    emailList.sourceModel = starredModel
                    selectedFolder = "starred"
                }
                onSentClicked:
                {
                    emailList.isDraftMode = false
                    emailList.sourceModel = sentModel
                    selectedFolder = "sent"
                }
                onDraftClicked:
                {
                    emailList.isDraftMode = true
                    emailList.sourceModel = draftModel
                    selectedFolder = "drafts"
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

    MultiEffect {
        id: blurEffect
        anchors.fill: mainContent
        source: mainContent

        blur: settingsLoader.opacity * 0.5
        blurEnabled: settingsLoader.active
        autoPaddingEnabled: false

        Behavior on blur {
            NumberAnimation { duration: 200 }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#000000"
        opacity: settingsLoader.opacity * 0.4
        visible: settingsLoader.active
        z:998
        MouseArea
        {

            hoverEnabled: true
            preventStealing: true
            enabled: parent.visible

            anchors.fill: parent
            onPressed: (mouse) => mouse.accepted = true
            onReleased: (mouse) => mouse.accepted = true
            onClicked: (mouse) => mouse.accepted = true
            onDoubleClicked: (mouse) => mouse.accepted = true
            onPositionChanged: (mouse) => mouse.accepted = true


            onWheel: (wheel) => {
                wheel.accepted = true
            }
        }
    }

    Loader {
        id: settingsLoader
        active: false
        z: 1000

        anchors.centerIn: parent

        width: item ? item.implicitWidth : 0
        height: item ? item.implicitHeight : 0

        Shortcut {
            sequence: "Escape"
            enabled: settingsLoader.active
            onActivated: {
                closeSettingsWindow()
            }
        }

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
        opacity: status === Loader.Ready ? 1 : 0
    }
}