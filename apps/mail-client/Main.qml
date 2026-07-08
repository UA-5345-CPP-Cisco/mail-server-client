import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import ISXMail 1.0

ApplicationWindow {

    //blocks
    id: window

    property string amountText: emailList.sourceModel ? emailList.sourceModel.pageAmountText : "0-0 of 0"
    property alias authLoader: authLoader
    property int draftsCount: draftModel.totalEmailsCount
    property int inboxCount: inboxModel.totalEmailsCount
    property int archiveCount: archiveModel.totalEmailsCount
    property var selectedEmail: null
    //properties
    property string selectedFolder: "inbox"
    property int sentCount: sentModel.totalEmailsCount
    property int starredCount: starredModel.totalEmailsCount

    // for adding avatar
    function avatarInitial(name) {
        var trimmedName = String(name).trim();
        return trimmedName.length > 0 ? trimmedName.charAt(0).toUpperCase() : "?";
    }
    function closeAuthWindow()
    {
        authLoader.active = false;
        authLoader.source = "";
    }

    //FunctionSorter
    function closeMessageWindow()
    {
        newMessageLoader.active = false;
        newMessageLoader.source = "";
    }
    function closeSettingsWindow()
    {
        settingsLoader.active = false;
        settingsLoader.source = "";
    }
    function formatEmailTime(input_time)
    {
        let message_date = parseDatabaseTimestamp(input_time);
        if (!message_date)
            return String(input_time);

        let current_date = new Date();

        let is_same_day = (message_date.getDate() === current_date.getDate() && message_date.getMonth() === current_date.getMonth() && message_date.getFullYear() === current_date.getFullYear());

        if (is_same_day)
        {
            return Qt.formatDateTime(message_date, "hh:mm");
        }
        else
        {
            return Qt.formatDateTime(message_date, "MMMM dd").toLowerCase();
        }
    }
    function formatEmailTimeFull(input_time)
    {
        let message_date = parseDatabaseTimestamp(input_time);
        if (!message_date)
            return String(input_time);

        let time_formatted = Qt.formatDateTime(message_date, "hh:mm");

        let date_formatted = Qt.formatDateTime(message_date, "MMMM dd").toLowerCase();

        return time_formatted + ", " + date_formatted;
    }
    function parseDatabaseTimestamp(input_time)
    {
        if (!input_time)
        {
            return null;
        }

        if (input_time instanceof Date)
        {
            return input_time;
        }

        let raw = String(input_time).trim();
        if (raw === "")
        {
            return null;
        }

        // SQLite CURRENT_TIMESTAMP is stored as UTC text: "YYYY-MM-DD HH:MM:SS".
        let utcMatch = raw.match(/^(\d{4})-(\d{2})-(\d{2})[ T](\d{2}):(\d{2}):(\d{2})(?:\.\d+)?$/);
        if (utcMatch)
        {
            return new Date(Date.UTC(parseInt(utcMatch[1]), parseInt(utcMatch[2]) - 1, parseInt(utcMatch[3]), parseInt(utcMatch[4]), parseInt(utcMatch[5]), parseInt(utcMatch[6])));
        }

        let parsed = new Date(raw);
        if (!isNaN(parsed.getTime()))
        {
            return parsed;
        }

        return null;
    }

    height: 768
    minimumHeight: 500
    minimumWidth: 750
    title: "Mail Client Interface"
    visible: true
    width: 1024

    MouseArea {
        anchors.fill: parent

        onClicked:
        {
            window.contentItem.forceActiveFocus();
        }
    }

    //new message loader
    Loader
    {
        id: newMessageLoader

        property var selectedItem: null

        active: false
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: item ? item.implicitHeight : 0
        opacity: status === Loader.Ready ? 1 : 0
        source: ""
        width: item ? item.implicitWidth : 0
        z: 999

        Behavior on opacity
        {
            NumberAnimation
            {
                duration: 200
            }
        }

        onLoaded:
        {
            connections.target = item;
            if (item && selectedItem) {
                item.newRecipient = selectedItem.sendTo;
                item.newSubject = selectedItem.subject;
                item.newText = selectedItem.content;
                item.newIndex = selectedItem.index;

                item.isDraft = selectedItem.isDraft === true || selectedItem.IsDraft === true;
                item.isReply = selectedItem.isReply === true || selectedItem.IsReply === true;
                item.isForward = selectedItem.isForward === true || selectedItem.IsForward === true;

                item.newTitle = selectedItem.newTitle !== undefined ? selectedItem.newTitle : selectedItem.title;
            }
            else
            {
                item.newRecipient = "";
                item.newSubject = "";
                item.newText = "";
                item.newIndex = "";
                item.isDraft = false;
                item.isReply = false;
                item.isForward = false;
                item.newTitle = "New Message";
            }
        }

        Connections
        {
            id: connections

            function onDraftChanged(index, subject, recipient, text)
            {
                if (newMessageLoader.selectedItem != null)
                {
                    draftModel.SetEmailData(parseInt(index), recipient, parseInt(EmailRole.SendToRole));
                    draftModel.SetEmailData(parseInt(index), subject, parseInt(EmailRole.ThemeRole));
                    draftModel.SetEmailData(parseInt(index), text, parseInt(EmailRole.ContentRole));
                    draftModel.SetEmailData(parseInt(index), text, parseInt(EmailRole.PreviewRole));
                }

                newMessageLoader.selectedItem = null;
            }
            function onDraftFinished(index, subject, recipient, text)
            {
                if (newMessageLoader.selectedItem != null)
                {
                    draftModel.RemoveEmailData(parseInt(index));
                    emailsModel.AddData(false, true, false, false, subject, CurrentUser.username, recipient, text, "");
                }

                newMessageLoader.selectedItem = null;
            }

            target: null
        }
    }
    RowLayout
    {
        anchors.fill: parent
        spacing: 0

        SplitView
        {
            id: splitView

            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: Qt.Horizontal

            handle: Rectangle
            {
                id: handleDelegate

                color: Color.transparent

                containmentMask: Item
                {
                    height: splitView.height
                    width: 5
                    x: (handleDelegate.width - width) / 2
                }
            }

            NavigationQML
            {
                id: navMenu

                Layout.fillHeight: true
                Layout.preferredWidth: 250
                SplitView.fillHeight: true
                SplitView.maximumWidth: 350
                SplitView.minimumWidth: 180
                SplitView.preferredWidth: 250

                onDraftClicked:
                {
                    emailList.isDraftMode = true;
                    emailList.sourceModel = draftModel;
                    window.selectedEmail = null;
                    selectedFolder = "drafts";
                }
                onArchiveClicked: {
                    emailList.isDraftMode = false;
                    emailList.sourceModel = archiveModel;
                    window.selectedEmail = null;
                    selectedFolder = "archive";
                }
                onInboxClicked:
                {
                    emailList.isDraftMode = false;
                    emailList.sourceModel = inboxModel;
                    window.selectedEmail = null;
                    selectedFolder = "inbox";
                }
                onSentClicked:
                {
                    emailList.isDraftMode = false;
                    emailList.sourceModel = sentModel;
                    window.selectedEmail = null;
                    selectedFolder = "sent";
                }
                onStarredClicked:
                {
                    emailList.isDraftMode = false;
                    emailList.sourceModel = starredModel;
                    window.selectedEmail = null;
                    selectedFolder = "starred";
                }
            }
            EmailsListQML
            {
                id: emailList

                SplitView.fillHeight: true
                SplitView.minimumWidth: 250
                SplitView.preferredWidth: 350

                onEmailOpenRequested: function (index, theme, name, sendTo, content, time, starred) {
                    window.selectedEmail =
                    {
                        "index": index,
                        "theme": theme,
                        "name": name,
                        "sendTo": sendTo,
                        "content": content,
                        "time": time,
                        "starred": starred
                    };
                }
            }
            Item
            {
                SplitView.fillHeight: true
                SplitView.fillWidth: true
                SplitView.minimumWidth: 250

                ContentBlankPageQML
                {
                    anchors.fill: parent
                    visible: window.selectedEmail === null
                }
                ContentPageLetterQML
                {
                    anchors.fill: parent
                    letterContent: window.selectedEmail ? window.selectedEmail.content : ""
                    letterIndex: window.selectedEmail ? window.selectedEmail.index : ""
                    letterName: window.selectedEmail ? window.selectedEmail.name : ""
                    letterSendTo: window.selectedEmail ? window.selectedEmail.sendTo : ""
                    letterStarred: window.selectedEmail ? window.selectedEmail.starred : false
                    letterTheme: window.selectedEmail ? window.selectedEmail.theme : ""
                    letterTime: window.selectedEmail ? window.selectedEmail.time : ""
                    visible: window.selectedEmail !== null

                    onDeleteClicked:
                    {
                        if (emailList.sourceModel)
                            emailList.sourceModel.RemoveEmailData(parseInt(letterIndex));
                        window.selectedEmail = null;
                    }
                    onArchiveClicked: {
                        if (emailList.sourceModel)
                            emailList.sourceModel.ToggleArchive(parseInt(letterIndex));
                    }
                    onStarClicked:
                    {
                        if (emailList.sourceModel)
                            emailList.sourceModel.SetStarred(parseInt(letterIndex), starred);
                        if (window.selectedEmail)
                        {
                            window.selectedEmail =
                                {
                                "index": window.selectedEmail.index,
                                "theme": window.selectedEmail.theme,
                                "name": window.selectedEmail.name,
                                "sendTo": window.selectedEmail.sendTo,
                                "content": window.selectedEmail.content,
                                "time": window.selectedEmail.time,
                                "starred": starred
                            };
                        }
                    }
                }
            }
        }
    }

    //blocker
    Rectangle
    {
        anchors.fill: parent
        color: Color.transparent
        opacity: settingsLoader.opacity * 0.4
        visible: settingsLoader.active
        z: 998

        MouseArea
        {
            anchors.fill: parent
            enabled: parent.visible
            hoverEnabled: true
            preventStealing: true

            onClicked: mouse => mouse.accepted = true
            onDoubleClicked: mouse => mouse.accepted = true
            onPositionChanged: mouse => mouse.accepted = true
            onPressed: mouse => mouse.accepted = true
            onReleased: mouse => mouse.accepted = true
            onWheel: wheel =>
            {
                wheel.accepted = true;
            }
        }
    }
    Rectangle
    {
        anchors.fill: parent
        color: Color.transparent
        opacity: authLoader.opacity * 0.4
        visible: authLoader.active
        z: 998

        MouseArea
        {
            anchors.fill: parent
            enabled: parent.visible
            hoverEnabled: true
            preventStealing: true

            onClicked: mouse => mouse.accepted = true
            onDoubleClicked: mouse => mouse.accepted = true
            onPositionChanged: mouse => mouse.accepted = true
            onPressed: mouse => mouse.accepted = true
            onReleased: mouse => mouse.accepted = true
            onWheel: wheel =>
                {
                wheel.accepted = true;
            }
        }
    }

    //settings loader
    Loader
    {
        id: settingsLoader

        active: false
        anchors.centerIn: parent
        height: item ? item.implicitHeight : 0
        opacity: status === Loader.Ready ? 1 : 0
        width: item ? item.implicitWidth : 0
        z: 1000

        Behavior on opacity
        {
            NumberAnimation
            {
                duration: 200
            }
        }

        Shortcut
        {
            enabled: settingsLoader.active
            sequence: "Escape"

            onActivated:
            {
                closeSettingsWindow();
            }
        }
    }

    // Loader for account change
    Loader
    {
        id: authLoader

        active: initialSetupRequired
        anchors.centerIn: parent
        height: item ? item.implicitHeight : 0
        opacity: status === Loader.Ready ? 1 : 0
        source: initialSetupRequired ? "screens/navigation/account/AddAccountQML.qml" : ""
        width: item ? item.implicitWidth : 0
        z: 1000

        Behavior on opacity
        {
            NumberAnimation
            {
                duration: 200
            }
        }

        Shortcut
        {
            enabled: authLoader.active
            sequence: "Escape"

            onActivated:
            {
                closeAuthWindow();
            }
        }
    }
}
