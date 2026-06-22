import QtQuick
import QtQuick.Effects
import QtQuick.Shapes

Rectangle
{
    id: delegateRoot
    height: 72
    color: "transparent"
    anchors.left: parent.left
    anchors.right: parent.right

    required property string accountName
    required property string accountEmail
    required property string avatarUrl
    required property string avatarColor
    required property string avatarInitial
    required property bool isActive
    required property int index

    Rectangle
    {
        id: buttonToSelectAccount
        x: 8
        y: 8
        width: parent.width - 16
        height: 56
        radius: 10
        color: delegateMouseArea.containsMouse ? "#f9fafb" : "transparent"

        // Аватарка
        Rectangle
        {
            id: avatarContainer
            x: 12
            y: 10
            width: 36
            height: 36
            radius: 18
            color: avatarUrl !== "" ? "transparent" : avatarColor

            // Зображення якщо є
            Image
            {
                anchors.fill: parent
                source: avatarUrl !== "" ? avatarUrl : ""
                visible: avatarUrl !== ""
                fillMode: Image.PreserveAspectCrop
                layer.enabled: true
                layer.effect: Component
                {
                    MultiEffect
                    {
                        maskEnabled: true
                        maskSource: ShaderEffectSource
                        {
                            sourceItem: Rectangle
                            {
                                width: avatarContainer.width
                                height: avatarContainer.height
                                radius: avatarContainer.radius
                            }
                        }
                    }
                }
            }

            // Фоллбек — ініціал
            Text
            {
                anchors.centerIn: parent
                visible: avatarUrl === ""
                text: avatarInitial
                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Black
            }
        }

        // Ім'я + email
        Column
        {
            x: 60
            y: 10
            width: parent.width - 88
            spacing: 0

            Text
            {
                width: parent.width
                height: 20
                text: accountName
                color: "#101828"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            Text
            {
                width: parent.width
                height: 16
                text: accountEmail
                color: "#6a7282"
                font.family: "Segoe UI"
                font.pixelSize: 12
                font.weight: Font.Normal
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }

        // Галочка активного акаунту
        Shape
        {
            x: parent.width - 28
            y: 20
            width: 16
            height: 16
            visible: isActive

            ShapePath
            {
                fillColor: "transparent"
                strokeColor: "#2b7fff"
                strokeWidth: 1.33
                PathSvg { path: "M 10.67 0 L 3.33 7.33 L 0 4" }
            }
        }

        MouseArea
        {
            id: delegateMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked:
            {
                currentUser.authorize
                (
                    accountName,
                    accountEmail,
                    avatarUrl
                )

                accountModel.SetActiveAccount(index)
            }
        }
    }
}

