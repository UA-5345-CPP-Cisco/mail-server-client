import QtQuick
import QtQuick.Effects
import QtQuick.Shapes

// SwitchAccountQML.qml
Rectangle
{
    id: switchAccountQML
    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            switchAccountQML.forceActiveFocus()
        }
    }


    implicitWidth: 250
    implicitHeight: header.height + accountList.contentHeight + addButton.height + 16

    clip: false
    color: "#ffffff"
    radius: 14

    // Header
    Rectangle
    {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 52
        color: "#00000000"

        Text
        {
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
            x: 254
            y: 18
            width: 16
            height: 16
            color: "transparent"

            Image
            {
                anchors.centerIn: parent
                source: "qrc:/pngs/assets/ic_close_window_black.svg"
                width: 15
                height: 15
                sourceSize.width: width * Screen.devicePixelRatio
                sourceSize.height: height * Screen.devicePixelRatio
                fillMode: Image.PreserveAspectFit
            }

            scale: closeMouse.containsMouse ? 1.3 : 1.0

            Behavior on scale
            {
                NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
            }

            MouseArea
            {
                id:closeMouse
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: switchAccountQML.parent.source = ""
            }
        }
    }

    // Розділювач після header
    Rectangle
    {
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 1
        anchors.rightMargin: 1
        height: 1
        color: "#e5e7eb"
    }

    // Список акаунтів
    ListView
    {
        id: accountList

        property int maxHeight: 214
        anchors.top: header.bottom
        anchors.topMargin: 1
        anchors.left: parent.left
        anchors.right: parent.right

        height: Math.min(maxHeight, contentHeight)
        interactive: contentHeight > maxHeight
        clip: true

        model: accountModel

        delegate: AccountItem {}
    }

    // Розділювач перед кнопкою
    Rectangle
    {
        id: divider
        anchors.top: accountList.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 1
        anchors.rightMargin: 1
        height: 1
        color: "#e5e7eb"
    }

    // Кнопка "Add account"
    Rectangle
    {
        id: addButton
        anchors.top: divider.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 72
        color: "transparent"

        Rectangle
        {
            x: 8
            y: 8
            width: parent.width - 16
            height: 56
            radius: 10
            color: addMouseArea.containsMouse ? "#f9fafb" : "transparent"

            // Іконка аватарки з плюсом
            Rectangle
            {
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
                    source: "qrc:/pngs/assets/ic_plus.svg"
                    width: 12
                    height: 12
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    scale: addMouseArea.containsMouse ? 1.3 : 1.0

                    Behavior on scale
                    {
                        NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
                    }
                }
            }

            Text
            {
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
                id: addMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                // onClicked: AccountController.startOAuth()
                onClicked: loginPopup.open()
            }
            AddAccountQML
            {
                id: loginPopup
            }
        }
    }
}
