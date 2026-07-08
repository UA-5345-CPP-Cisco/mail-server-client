import QtQuick
import QtQuick.Effects
import QtQuick.Shapes

Rectangle
{
    id: rootRectangle
    height: 72
    color: Color.transparent
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
        id: selectAccountButtonRectangle
        x: 8
        y: 8
        width: parent.width - 16
        height: 56
        radius: 10
        color: delegateClickArea.containsMouse ? Color.surface : Color.transparent

        // Avatar wrapper for added users
        Rectangle
        {
            id: avatarContainerRectangle
            x: 12
            y: 10
            width: 36
            height: 36
            radius: 18
            color: avatarUrl !== "" ? Color.transparent : avatarColor

            Image
            {
                id: avatarIcon
                anchors.fill: parent
                source: avatarUrl !== "" ? avatarUrl : ""
                visible: avatarUrl !== ""
                fillMode: Image.PreserveAspectCrop
                layer.enabled: true

                layer.effect: Component
                {
                    id: maskComponent

                    MultiEffect
                    {
                        id: roundMaskEffect
                        maskEnabled: true

                        maskSource: ShaderEffectSource
                        {
                            id: maskShaderSource
                            sourceItem: Rectangle
                            {
                                id: maskShapeRectangle
                                width: avatarContainerRectangle.width
                                height: avatarContainerRectangle.height
                                radius: avatarContainerRectangle.radius
                            }
                        }
                    }
                }
            }

            // Text letter (shown if no image)
            Text
            {
                id: fallbackInitialText
                anchors.centerIn: parent
                visible: avatarUrl === ""
                text: avatarInitial
                color: Color.background
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Black
            }
        }

        // User informations
        Column
        {
            id: accountTextDataColumn
            x: 60
            y: 10
            width: parent.width - 88
            spacing: 0

            Text
            {
                id: accountNameText
                width: parent.width
                height: 20
                text: accountName
                color: Color.button
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            Text
            {
                id: accountEmailText
                width: parent.width
                height: 16
                text: accountEmail
                color: Color.secondaryText
                font.family: "Segoe UI"
                font.pixelSize: 12
                font.weight: Font.Normal
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }

        // Checkmark for active account
        Image
        {
            id: activeCheckmarkIcon
            x: parent.width - 28
            y: 20
            width: 16
            height: 16
            visible: isActive
            source: "qrc:/pngs/assets/ic_checkmark.svg"
            sourceSize.width: width * Screen.devicePixelRatio
            sourceSize.height: height * Screen.devicePixelRatio
            fillMode: Image.PreserveAspectFit
        }

        // Handles account selection clicks and hover effects
        MouseArea
        {
            id: delegateClickArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onClicked:
            {
                CurrentUser.Authorize(accountName, accountEmail, avatarUrl)
                accountModel.SetActiveAccount(index)
            }
        }
    }
}