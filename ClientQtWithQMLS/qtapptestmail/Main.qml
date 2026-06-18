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
    property var selectedEmail: null

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

    function format_email_time(input_time)
    {
        if (!input_time)
        {
            return "";
        }

        // Automatically parse if input arrives as a string, or preserve if it is already a JS Date
        let message_date = (input_time instanceof Date) ? input_time : new Date(input_time);

        // Fallback security check: if parsing failed, return raw string
        if (isNaN(message_date.getTime()))
        {
            return String(input_time);
        }

        let current_date = new Date();

        let is_same_day = (message_date.getDate() === current_date.getDate() &&
                           message_date.getMonth() === current_date.getMonth() &&
                           message_date.getFullYear() === current_date.getFullYear());

        if (is_same_day)
        {
            // Today: displays time format like "10:30 AM"
            return Qt.formatDateTime(message_date, "hh:mm AP");
        }
        else
        {
            // Different day: displays "06 august" (lowercased)
            return Qt.formatDateTime(message_date, "dd mmmm").toLowerCase();
        }
    }
    function format_email_time_full(input_time)
    {
        if (!input_time)
        {
            return "";
        }

        let message_date = (input_time instanceof Date) ? input_time : new Date(input_time);

        if (isNaN(message_date.getTime()))
        {
            return String(input_time);
        }

        let time_formatted = Qt.formatDateTime(message_date, "hh:mm AP");

        let date_formatted = Qt.formatDateTime(message_date, "dd mmmm").toLowerCase();

        return time_formatted + ", " + date_formatted;
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
        property var selectedItem: null
        Behavior on opacity
        {
            NumberAnimation { duration: 200 }
        }

        onLoaded: {
                   connections.target = item
                   if (item && selectedItem)
                   {
                       item.newRecipient = selectedItem.sendTo
                       item.newSubject = selectedItem.subject
                       item.newText = selectedItem.content
                       item.newIndex = selectedItem.index
                       item.isDraft = selectedItem.isDraft === true
                       item.newTitle = item.isDraft ? "Draft" : "New Message"
                   }
                   else
                    {
                        item.newRecipient = ""
                        item.newSubject = ""
                        item.newText = ""
                        item.newIndex = ""
                        item.isDraft = false
                        item.newTitle = "New Message"
                    }
               }

               Connections {
                   id: connections
                   target: null

                   function onDraftChanged(index, subject, recipient, text) {
                       if(newMessageLoader.selectedItem != null)
                       {
                           draftModel.setEmailData(parseInt(index), recipient, parseInt(EmailRole.SendToRole))
                           draftModel.setEmailData(parseInt(index), subject, parseInt(EmailRole.ThemeRole))
                           draftModel.setEmailData(parseInt(index), text, parseInt(EmailRole.ContentRole))
                       }

                       newMessageLoader.selectedItem = null
                   }

                   function onDraftFinished(index, subject, recipient, text) {
                       if(newMessageLoader.selectedItem != null)
                       {
                           draftModel.removeEmailData(parseInt(index))
                           emailsModel.AddData(
                               false, true, false,
                               subject, currentUser.username,
                               recipient, text, ""
                           )
                       }

                       newMessageLoader.selectedItem = null
                   }
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
                    window.selectedEmail = null
                    selectedFolder = "inbox"
                }
                onStarredClicked:
                {
                    emailList.isDraftMode = false
                    emailList.sourceModel = starredModel
                    window.selectedEmail = null
                    selectedFolder = "starred"
                }
                onSentClicked:
                {
                    emailList.isDraftMode = false
                    emailList.sourceModel = sentModel
                    window.selectedEmail = null
                    selectedFolder = "sent"
                }
                onDraftClicked:
                {
                    emailList.isDraftMode = true
                    emailList.sourceModel = draftModel
                    window.selectedEmail = null
                    selectedFolder = "drafts"
                }
            }
            EmailsListQML
            {
                id:emailList
                SplitView.preferredWidth: 350
                SplitView.minimumWidth: 250
                SplitView.fillHeight: true
                onEmailOpenRequested: function(index, theme, name, sendTo, content, time, starred) {
                    window.selectedEmail = {
                        "index": index,
                        "theme": theme,
                        "name": name,
                        "sendTo": sendTo,
                        "content": content,
                        "time": time,
                        "starred": starred
                    }
                }
            }

            Item
            {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                SplitView.minimumWidth: 250

                ContentBlankPageQML
                {
                    anchors.fill: parent
                    visible: window.selectedEmail === null
                }

                ContentPageLetterQML
                {
                    anchors.fill: parent
                    visible: window.selectedEmail !== null

                    letterIndex: window.selectedEmail ? window.selectedEmail.index : ""
                    letterTheme: window.selectedEmail ? window.selectedEmail.theme : ""
                    letterName: window.selectedEmail ? window.selectedEmail.name : ""
                    letterSendTo: window.selectedEmail ? window.selectedEmail.sendTo : ""
                    letterContent: window.selectedEmail ? window.selectedEmail.content : ""
                    letterTime: window.selectedEmail ? window.selectedEmail.time : ""
                    letterStarred: window.selectedEmail ? window.selectedEmail.starred : false

                    onStarClicked:
                    {
                        if (emailList.sourceModel)
                            emailList.sourceModel.setStarred(parseInt(letterIndex), starred)
                        if (window.selectedEmail)
                        {
                            window.selectedEmail = {
                                "index": window.selectedEmail.index,
                                "theme": window.selectedEmail.theme,
                                "name": window.selectedEmail.name,
                                "sendTo": window.selectedEmail.sendTo,
                                "content": window.selectedEmail.content,
                                "time": window.selectedEmail.time,
                                "starred": starred
                            }
                        }
                    }

                    onDeleteClicked:
                    {
                        if (emailList.sourceModel)
                            emailList.sourceModel.removeEmailData(parseInt(letterIndex))
                        window.selectedEmail = null
                    }
                }
            }
        }
    }

    MultiEffect
    {
        id: blurEffect
        anchors.fill: splitView
        source: splitView

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

    Loader
    {
        id: settingsLoader
        active: false
        z: 1000

        anchors.centerIn: parent

        width: item ? item.implicitWidth : 0
        height: item ? item.implicitHeight : 0

        Shortcut
        {
            sequence: "Escape"
            enabled: settingsLoader.active
            onActivated:
            {
                closeSettingsWindow()
            }
        }

        Behavior on opacity
        {
            NumberAnimation { duration: 200 }
        }
        opacity: status === Loader.Ready ? 1 : 0
    }
}
