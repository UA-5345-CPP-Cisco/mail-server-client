import QtQuick
import QtQuick.Shapes
import QtQuick.Controls

Rectangle {
    id: emailsListQML

    property bool isDraftMode: false
    property var sourceModel: inboxModel

    signal emailOpenRequested(int index, string theme, string name, string sendTo, string content, string time, bool starred)
    signal starredItemClicked(bool starred)

    function activeSearchModel() {
        if (sourceModel === inboxModel)
            return inboxSearchModel;
        if (sourceModel === sentModel)
            return sentSearchModel;
        if (sourceModel === starredModel)
            return starredSearchModel;
        if (sourceModel === draftModel)
            return draftSearchModel;
        return inboxSearchModel;
    }

    color: Color.background

    onSourceModelChanged: {
        var searchProxy = activeSearchModel();
        if (searchProxy && searchTextField.text !== searchProxy.SearchedText)
            searchTextField.text = searchProxy.SearchedText;
    }

    Component {
        id: draftDelegate

        DraftItem {
            content: parent.pContent
            name: parent.pName
            preview: parent.pPreview
            searchModel: parent.pSearchModel
            sendTo: parent.pSendTo
            theme: parent.pTheme
            time: parent.pTime
            width: parent.width

            onClicked: {
                newMessageLoader.selectedItem = {
                    "index": parent.pIndex,
                    "subject": theme,
                    "sendTo": sendTo,
                    "content": content,
                    "isDraft": true,
                    "IsForward": false,
                    "isReply": false,
                    "title": "Draft",
                    "newTitle": "Draft"
                };

                newMessageLoader.source = "";
                newMessageLoader.active = false;

                newMessageLoader.active = true;
                newMessageLoader.source = "screens/navigation/new_message/NewMessageQML.qml";
            }
            onDeleteClicked: {
                if (sourceModel)
                    sourceModel.RemoveEmailData(parent.pIndex);
            }
            onOpenRequested: function (theme, name, sendTo, content, time) {
                emailsListQML.emailOpenRequested(parent.pIndex, theme, name, sendTo, content, time, false);
            }
        }
    }
    Component {
        id: emailsDelegate

        ListItem {
            content: parent.pContent
            name: parent.pName
            preview: parent.pPreview
            searchModel: parent.pSearchModel
            sendTo: parent.pSendTo
            starred: parent.pStarred
            theme: parent.pTheme
            time: formatEmailTime(parent.pTime)
            width: parent.width

            onDeleteClicked: {
                if (sourceModel)
                    sourceModel.RemoveEmailData(parent.pIndex);
            }
            onOpenRequested: function (theme, name, sendTo, content, time, starred) {
                emailsListQML.emailOpenRequested(parent.pIndex, theme, name, sendTo, content, parent.pTime, starred);
            }
            onStarredClicked: {
                sourceModel.SetStarred(parent.pIndex, starred);
                starredItemClicked(starred);
            }
        }
    }

    // Right border (handle of SplitView)
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.top: parent.top
        color: Color.outline
        width: 1
        z: 1
    }

    // Left border
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        color: Color.outline
        width: 1
        z: 1
    }

    // Search section
    Rectangle {
        id: searchSection

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        color: Color.background
        height: 70

        Rectangle {
            id: searchLine

            color: Color.transparent
            height: 38

            anchors {
                left: parent.left
                margins: 16
                right: parent.right
                top: parent.top
            }

            // Search line
            Rectangle {
                id: inputLine

                anchors.left: parent.left
                anchors.right: parent.right
                border.color: Color.border
                border.width: 1
                clip: true
                color: Color.background
                height: 38
                radius: 10

                Item {
                    id: searchIcon

                    height: 16
                    width: 16
                    x: 12
                    y: 11

                    Image {
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        height: 15
                        source: "qrc:/pngs/assets/ic_search.svg"
                        sourceSize.height: 120
                        sourceSize.width: 120
                        width: 15
                    }
                }
                Rectangle {
                    id: searchContainer

                    anchors.left: parent.left
                    anchors.leftMargin: 36.80
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    clip: true
                    color: Color.transparent
                    height: 19

                    TextField {
                        id: searchTextField

                        anchors.fill: parent
                        bottomPadding: 0
                        color:Color.hover
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 0
                        placeholderText: "Search mail"
                        placeholderTextColor: Color.secondaryText
                        topPadding: 0

                        background: Rectangle {
                            color: Color.transparent
                        }
                        cursorDelegate: Item {
                        }

                        onTextChanged: {
                            var searchProxy = emailsListQML.activeSearchModel();
                            if (searchProxy)
                                searchProxy.SearchedText = text;
                        }

                        Rectangle {
                            id: customCursorContainer

                            anchors.verticalCenter: parent.verticalCenter
                            color: Color.secondaryText
                            height: parent.font.pixelSize
                            visible: parent.activeFocus
                            width: 1
                            x: parent.cursorRectangle.x

                            // Smooth flashing
                            SequentialAnimation on opacity {
                                loops: Animation.Infinite

                                NumberAnimation {
                                    duration: 500
                                    easing.type: Easing.InOutSine
                                    to: 0
                                }
                                NumberAnimation {
                                    duration: 500
                                    easing.type: Easing.InOutSine
                                    to: 1
                                }
                            }

                            // Smooth right/left movement
                            Behavior on x {
                                NumberAnimation {
                                    duration: 80
                                    easing.type: Easing.OutCubic
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Inbox Separator
    Rectangle {
        id: separatorLine

        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: searchSection.bottom
        anchors.topMargin: 0
        color: Color.surface
        height: 34

        // Top border
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            color: Color.outline
            height: 1
        }

        // Bottom border
        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            color: Color.outline
            height: 1
        }
        Rectangle {
            id: separatorContainer

            color: Color.transparent
            height: 18

            anchors {
                left: parent.left
                margins: 16
                right: parent.right
                top: parent.top
            }

            // Inbox and page label
            Rectangle {
                id: textContainer

                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: navButtonContainer.left
                anchors.rightMargin: 0
                color: Color.transparent
                height: 18
                y: -8

                Text {
                    id: inboxText

                    color: Color.secondaryText
                    font.capitalization: Font.AllUppercase
                    font.family: "Segoe UI"
                    font.letterSpacing: 0.60
                    font.pixelSize: 12
                    font.weight: Font.Black
                    height: 16
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: selectedFolder
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: pageText

                    anchors.right: parent.right
                    color: Color.secondaryText
                    font.capitalization: Font.AllUppercase
                    font.family: "Segoe UI"
                    font.letterSpacing: 0.60
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    height: 16
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: amountText
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    y: 1
                }
            }

            // Navigation buttons
            Item {
                id: navButtonContainer

                anchors.right: parent.right
                anchors.rightMargin: -6
                height: 18
                width: 50
                x: 564
                y: -8

                // Button back
                Rectangle {
                    id: moveBackButtonContainer

                    anchors.left: parent.left
                    color: Color.transparent
                    height: 17
                    scale: moveBackButtonClickArea.containsMouse ? 1.3 : 1.0
                    width: 15

                    Behavior on scale {
                        NumberAnimation {
                            duration: 150
                            easing.type: Easing.InOutQuad
                        }
                    }

                    MouseArea {
                        id: moveBackButtonClickArea

                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true

                        onClicked: {
                            if (listView.model) {
                                listView.model.PrevPage();
                            }
                        }
                    }
                    Rectangle {
                        id: moveBackButton

                        clip: true
                        color: Color.transparent
                        height: 15
                        width: 15
                        y: 2

                        Image {
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                            height: 15
                            source: "qrc:/pngs/assets/ic_button_back.svg"
                            sourceSize.height: height * Screen.devicePixelRatio
                            sourceSize.width: width * Screen.devicePixelRatio
                            width: 15
                        }
                    }
                }

                // Button forward
                Rectangle {
                    id: moveForwardButtonContainer

                    anchors.right: parent.right
                    color: Color.transparent
                    height: 18
                    scale: moveForwardButtonClickArea.containsMouse ? 1.3 : 1.0
                    width: 15

                    Behavior on scale {
                        NumberAnimation {
                            duration: 150
                            easing.type: Easing.InOutQuad
                        }
                    }

                    MouseArea {
                        id: moveForwardButtonClickArea

                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true

                        onClicked: {
                            if (listView.model) {
                                listView.model.NextPage();
                            }
                        }
                    }
                    Rectangle {
                        id: moveForwardButton

                        clip: true
                        color: Color.transparent
                        height: 15
                        width: 15
                        y: 2

                        Image {
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                            height: 15
                            source: "qrc:/pngs/assets/ic_button_forward.svg"
                            sourceSize.height: height * Screen.devicePixelRatio
                            sourceSize.width: width * Screen.devicePixelRatio
                            width: 15
                        }
                    }
                }
            }
        }
    }
    HoverHandler {
        id: listViewHover
    }

    //Emails model connector
    ListView {
        id: listView

        clip: true
        model: sourceModel

        ScrollBar.vertical: ScrollBar {
            id: vBar

            active: true
            policy: ScrollBar.AsNeeded
            visible: (listView.contentHeight > listView.height) && listViewHover.hovered

            contentItem: Rectangle {
                color: Color.outline
                implicitHeight: 100
                implicitWidth: 6
                radius: 3
            }
        }
        delegate: Loader {
            property string pContent: emailsContent
            property int pIndex: index
            property string pName: emailsName
            property string pPreview: emailsPreview
            property var pSearchModel: emailsListQML.activeSearchModel()
            property string pSendTo: emailsSendTo
            property bool pStarred: emailsStarred
            property string pTheme: emailsTheme
            property string pTime: emailsTime

            sourceComponent: isDraftMode ? draftDelegate : emailsDelegate
            width: listView.width
        }

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            top: separatorLine.bottom
        }
    }
}
