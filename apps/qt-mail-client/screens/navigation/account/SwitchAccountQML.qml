import QtQuick
import QtQuick.Effects
import QtQuick.Shapes

Rectangle
{
    id: switchAccountRectangle

    implicitWidth: 250
    implicitHeight: headerRectangle.height + accountListView.contentHeight + addButtonRectangle.height + bottomSpacerRectangle.height + 16

    clip: false
    color: "#ffffff"
    radius: 14

    MouseArea
    {
        id: rootClickArea
        anchors.fill: parent

        onClicked:
        {
            switchAccountRectangle.forceActiveFocus()
        }
    }

    // Header
    Rectangle
    {
        id: headerRectangle
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 52
        color: "#00000000"

        Text
        {
            id: headerTitleText
            x: 16
            y: 16
            height: 20
            color: "#101828"
            font.family: "Segoe UI"
            font.pixelSize: 14
            font.weight: Font.Black
            text: "Switch Account"
            verticalAlignment: Text.AlignVCenter
        }

        Rectangle
        {
            id: closeButtonWrapperRectangle
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.verticalCenter: parent.verticalCenter
            width: 16
            height: 16
            color: "transparent"

            Image
            {
                id: closeIconImage
                anchors.centerIn: parent
                source: "qrc:/pngs/assets/ic_close_window_black.svg"
                width: 15
                height: 15
                sourceSize.width: width * Screen.devicePixelRatio
                sourceSize.height: height * Screen.devicePixelRatio
                fillMode: Image.PreserveAspectFit
            }

            scale: closeClickArea.containsMouse ? 1.3 : 1.0

            Behavior on scale
            {
                id: closeScaleBehavior
                NumberAnimation { id: closeScaleAnimation; duration: 150; easing.type: Easing.InOutQuad }
            }

            MouseArea
            {
                id: closeClickArea
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor

                onClicked:
                {
                    switchAccountRectangle.parent.source = ""
                }
            }
        }
    }

    // Divider after header
    Rectangle
    {
        id: headerDividerRectangle
        anchors.top: headerRectangle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 1
        anchors.rightMargin: 1
        height: 1
        color: "#e5e7eb"
    }

    // Account list
    ListView
    {
        id: accountListView

        property int maxHeight: 214
        anchors.top: headerRectangle.bottom
        anchors.topMargin: 1
        anchors.left: parent.left
        anchors.right: parent.right

        height: Math.min(maxHeight, contentHeight)
        interactive: contentHeight > maxHeight
        clip: true

        model: accountModel

        delegate: AccountItem {}
    }

    // Divider before button
    Rectangle
    {
        id: listDividerRectangle
        anchors.top: accountListView.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 1
        anchors.rightMargin: 1
        height: 1
        color: "#e5e7eb"
    }

    // "Add account" button
    Rectangle
    {
        id: addButtonRectangle
        anchors.top: listDividerRectangle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 72
        color: "transparent"

        signal addAccountRequested()

        Rectangle
        {
            id: innerButtonBlockRectangle
            x: 8
            y: 8
            width: parent.width - 16
            height: 56
            radius: 10
            color: addClickArea.containsMouse ? "#f9fafb" : "transparent"

            Rectangle
            {
                id: avatarWrapperRectangle
                x: 12
                y: 10
                width: 36
                height: 36
                radius: 18
                color: "transparent"
                border.color: "#e5e7eb"
                border.width: 1

                Image
                {
                    id: plusIconImage
                    source: "qrc:/pngs/assets/ic_plus.svg"
                    width: 12
                    height: 12
                    sourceSize.width: width * Screen.devicePixelRatio
                    sourceSize.height: height * Screen.devicePixelRatio
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    scale: addClickArea.containsMouse ? 1.3 : 1.0

                    Behavior on scale
                    {
                        id: plusScaleBehavior
                        NumberAnimation { id: plusScaleAnimation; duration: 150; easing.type: Easing.InOutQuad }
                    }
                }
            }

            Text
            {
                id: addButtonText
                x: 60
                y: 18
                height: 20
                text: "Add account"
                color: "#4a5565"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea
            {
                id: addClickArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked:
                {
                    authLoader.active = true
                    if (String( authLoader.source) === "")
                    {
                        authLoader.source = "screens/navigation/account/AddAccountQML.qml"
                    } else
                    {
                        authLoader.source = ""
                    }
                }
            }
        }
    }

    // Background padding block
    Rectangle
    {
        id: bottomSpacerRectangle
        anchors.top: addButtonRectangle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 300
        color: "#ffffff"

        MouseArea
        {
            id: spacerIgnoreClickArea
            anchors.fill: parent
            hoverEnabled: true

            onClicked:
            {
                // Intentional no-op to capture background clicks
            }
        }
    }
}