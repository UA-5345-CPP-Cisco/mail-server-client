import QtQuick
import QtQuick.Shapes
import QtQuick.Controls
import ISXMail 1.0

Rectangle
{
    id: emailsListQML
    color: "#ffffff"

    property bool isDraftMode: false
    property var sourceModel: inboxModel
    signal emailOpenRequested(string theme, string name, string sendTo, string content, string time, bool starred)

    function activeSearchModel() {
        if (sourceModel === inboxModel)
            return inboxSearchModel
        if (sourceModel === sentModel)
            return sentSearchModel
        if (sourceModel === starredModel)
            return starredSearchModel
        if (sourceModel === draftModel)
            return draftSearchModel
        return inboxSearchModel
    }

    onSourceModelChanged: {
        var searchProxy = activeSearchModel()
        if (searchProxy && search_input.text !== searchProxy.searchedText)
            search_input.text = searchProxy.searchedText
    }

    Component {
        id: draftDelegate

        DraftItem {
            width: parent.width

            theme: parent.pTheme
            name: parent.pName
            preview: parent.pPreview
            content: parent.pContent
            sendTo: parent.pSendTo
            time: parent.pTime
            searchModel: parent.pSearchModel
            onOpenRequested: function(theme, name, sendTo, content, time) {
                emailsListQML.emailOpenRequested(theme, name, sendTo, content, time, false)
            }
        }
    }

    Component {
        id: emailsDelegate

        ListItem {
            width: parent.width

            theme: parent.pTheme
            name: parent.pName
            preview: parent.pPreview
            content: parent.pContent
            sendTo: parent.pSendTo
            time: parent.pTime
            starred: parent.pStarred
            searchModel: parent.pSearchModel

            onOpenRequested: function(theme, name, sendTo, content, time, starred) {
                emailsListQML.emailOpenRequested(theme, name, sendTo, content, time, starred)
            }

            onStarredClicked: {
                if(starred){
                    sourceModel.setEmailData(parent.pIndex, true, parseInt(EmailRole.StarredRole))
                }
                else{
                    sourceModel.setEmailData(parent.pIndex, false, parseInt(EmailRole.StarredRole))
                }
            }
        }
    }

    // Right border (handle of SplitView)
    Rectangle
    {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 1
        color: "#e5e7eb"
        z: 1
    }

    // Left border
    Rectangle
    {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 1
        color: "#e5e7eb"
        z:1
    }


    // Search section
    Rectangle
    {
        id: searchSection

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: 70
        color: "#ffffff"

        Rectangle
        {
            id: searchLine

            anchors
            {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: 16
            }

            height: 38
            color: "transparent"

            // Search line
            Rectangle
            {
                id: input

                anchors.left: parent.left
                anchors.right: parent.right
                height: 38

                border.color: "#d1d5dc"
                border.width: 1
                clip: true
                color: "#ffffff"
                radius: 10

                Item
                {
                    id: searchIconArea
                    x: 12
                    y: 11
                    width: 16
                    height: 16
                    Image
                    {
                        source: "qrc:/pngs/assets/ic_search.svg"
                        width: 15
                        height: 15
                        sourceSize.width: 120
                        sourceSize.height: 120
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                    }
                }

                // PlaceHolder
                Rectangle
                {
		    id: search_container
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 36.80
                    anchors.rightMargin: 8
                    height: 19
                    clip: true
                    color: "transparent"

                    TextField
                    {
                        id: search_input
                        anchors.fill: parent
                        color: "#1f2937"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        placeholderText: "Search mail"
                        placeholderTextColor: "#99a1af"

                        onTextChanged: {
                            var searchProxy = emailsListQML.activeSearchModel()
                            if (searchProxy)
                                searchProxy.searchedText = text
                        }

                        background: Item {}

                        leftPadding: 0
                        topPadding: 0
                        bottomPadding: 0
                        cursorDelegate: Item {}
                        Rectangle
                        {
                            id: custom_cursor_for_search_container
                            width: 1
                            color: "#1f2937"
                            height: parent.font.pixelSize
                            anchors.verticalCenter: parent.verticalCenter
                            x: parent.cursorRectangle.x
                            visible: parent.activeFocus

                            // Smooth right/left movement
                            Behavior on x
                            {
                                NumberAnimation
                                {
                                    duration: 80
                                    easing.type: Easing.OutCubic
                                }
                            }

                            // Smooth flashing
                            SequentialAnimation on opacity
                            {
                                loops: Animation.Infinite
                                NumberAnimation { to: 0; duration: 500; easing.type: Easing.InOutSine }
                                NumberAnimation { to: 1; duration: 500; easing.type: Easing.InOutSine }
                            }
                        }
                    }
                }
            }
        }
    }

    // Inbox Separator
    Rectangle
    {
        id: separator

        anchors.top:searchSection.bottom
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.right: parent.right

        height: 34
        color: "#f9fafb"

        // Top border
        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 1
            color: "#e5e7eb"
        }

        // Bottom border
        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 1
            color: "#e5e7eb"
        }

        Rectangle
        {
            id: heading_3
            anchors
            {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: 16
            }
            height: 18
            color: "transparent"

            // Inbox and page label
            Rectangle
            {
                id: frame_3
                y: -8
                height: 18
                anchors.left: parent.left
                anchors.right: navButtons.left
                anchors.leftMargin: 0
                anchors.rightMargin: 0
                color: "transparent"

                Text
                {
                    id: inbox
                    height: 16
                    color: "#6a7282"
                    font.capitalization: Font.AllUppercase
                    font.family: "Segoe UI"
                    font.letterSpacing: 0.60
                    font.pixelSize: 12
                    font.weight: Font.Black
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: selectedFolder
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }

                Text
                {
                    id: amount
                    anchors.right: parent.right
                    y: 1
                    height: 16
                    color: "#000000"
                    font.capitalization: Font.AllUppercase
                    font.family: "Segoe UI"
                    font.letterSpacing: 0.60
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: amountText
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }

            // Navigation buttons
            Item
            {
                id: navButtons
                x: 564
                y: -8
                anchors.right: parent.right
                anchors.rightMargin: -6
                width: 50
                height: 18

                // Button back
                Rectangle
                {
                    id: button_margin
                    anchors.left: parent.left
                    width: 15
                    height: 17
                    color: "transparent"
                    scale: clickAreaMoveBack.containsMouse ? 1.3 : 1.0

                    Behavior on scale
                    {
                        NumberAnimation
                        {
                            duration: 150
                            easing.type: Easing.InOutQuad
                        }
                    }
                    MouseArea
                    {
                        id: clickAreaMoveBack
                        anchors.fill: parent

                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked:
                        {
                                    if (listView.model)
                                        listView.model.prevPage()
                        }
                    }
                    Rectangle
                    {
                        id: buttonToMoveBack
                        y: 2
                        height: 15
                        width: 15
                        clip: true
                        color: "transparent"

                        Image
                        {
                            source: "qrc:/pngs/assets/ic_button_back.svg"
                            width: 15
                            height: 15
                            fillMode: Image.PreserveAspectFit
                            anchors.centerIn: parent
                        }
                    }
                }

                // Button forward
                Rectangle
                {
                    id: button_margin_1
                    anchors.right: parent.right
                    width: 15
                    height: 18
                    color: "transparent"
                    scale: clickAreaMoveForvard.containsMouse ? 1.3 : 1.0
                    MouseArea {
                        id: clickAreaMoveForvard
                        anchors.fill: parent

                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: {
                            if (listView.model)
                                listView.model.nextPage()
                        }
                    }

                    Behavior on scale
                    {
                        NumberAnimation
                        {
                            duration: 150
                            easing.type: Easing.InOutQuad
                        }
                    }

                    Rectangle
                    {
                        id: buttonToMoveForward
                        y: 2
                        height: 15
                        width: 15
                        clip: true
                        color: "transparent"

                        Image
                        {
                            source: "qrc:/pngs/assets/ic_button_forward.svg"
                            width: 15
                            height: 15
                            sourceSize.width: 120
                            sourceSize.height: 120
                            fillMode: Image.PreserveAspectFit
                            anchors.centerIn: parent
                        }
                    }
                }
            }
        }
    }
    //Emails model conector
    ListView
    {
        id:listView
        anchors
        {
            top:separator.bottom;
            right: parent.right;
            left: parent.left;
            bottom: parent.bottom;
        }

        model: sourceModel

        delegate: Loader
        {
            width: listView.width

            property string pTheme: emailsTheme
            property string pName: emailsName
            property string pSendTo: emailsSendTo
            property string pPreview: emailsPreview
            property string pContent: emailsContent
            property string pTime: emailsTime
            property bool pStarred: emailsStarred
            property var pSearchModel: emailsListQML.activeSearchModel()
            property var pIndex: index

            sourceComponent: isDraftMode
                             ? draftDelegate
                             : emailsDelegate
        }
    }
}
