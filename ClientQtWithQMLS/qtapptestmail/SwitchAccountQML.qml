import QtQuick
import QtQuick.Effects
import QtQuick.Shapes

// SwitchAccountQML.qml
Rectangle {
    id: switchAccountQML

    implicitWidth: 287
    implicitHeight: header.height + accountList.contentHeight + addButton.height + 16

    clip: false
    color: "#ffffff"
    radius: 14

    // Header
    Rectangle {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 52
        color: "transparent"

        Text {
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

        Rectangle {
            x: 254
            y: 18
            width: 16
            height: 16
            color: "transparent"

            Shape {
                ShapePath {
                    fillColor: "transparent"
                    strokeColor: "#6a7282"
                    strokeWidth: 1.33
                    PathSvg { path: "M 8 0 L 0 8" }
                }
            }
            Shape {
                ShapePath {
                    fillColor: "transparent"
                    strokeColor: "#6a7282"
                    strokeWidth: 1.33
                    PathSvg { path: "M 0 0 L 8 8" }
                }
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: switchAccountQML.parent.source = ""
            }
        }
    }

    // Розділювач після header
    Rectangle {
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 1
        anchors.rightMargin: 1
        height: 1
        color: "#e5e7eb"
    }

    // Список акаунтів
    ListView {
        id: accountList

        anchors.top: header.bottom
        anchors.topMargin: 1
        anchors.left: parent.left
        anchors.right: parent.right

        height: contentHeight
        interactive: false
        clip: false

        // Тимчасова модель — замінити на AccountController.accountModel
        model: ListModel {
            ListElement {
                accountName: "Personal"
                accountEmail: "alexm@gmail.com"
                avatarUrl: ""        // шлях до зображення або ""
                avatarColor: "#2b7fff"
                avatarInitial: "P"
                isActive: true
            }
            ListElement {
                accountName: "Work"
                accountEmail: "alex@company.com"
                avatarUrl: ""
                avatarColor: "#7c3aed"
                avatarInitial: "W"
                isActive: false
            }
        }

        delegate: Rectangle {
            id: delegateRoot
            width: accountList.width
            height: 72
            color: "transparent"

            Rectangle {
                id: buttonToSelectAccount
                x: 8
                y: 8
                width: parent.width - 16
                height: 56
                radius: 10
                color: delegateMouseArea.containsMouse ? "#f9fafb" : "transparent"

                // Аватарка
                Rectangle {
                    id: avatarContainer
                    x: 12
                    y: 10
                    width: 36
                    height: 36
                    radius: 18
                    color: model.avatarUrl !== "" ? "transparent" : model.avatarColor

                    // Зображення якщо є
                    Image {
                        anchors.fill: parent
                        source: model.avatarUrl !== "" ? model.avatarUrl : ""
                        visible: model.avatarUrl !== ""
                        fillMode: Image.PreserveAspectCrop
                        layer.enabled: true
                        layer.effect: Component {
                                MultiEffect {
                                    maskEnabled: true
                                    maskSource: ShaderEffectSource {
                                        sourceItem: Rectangle {
                                            width: avatarContainer.width
                                            height: avatarContainer.height
                                            radius: avatarContainer.radius
                                        }
                                    }
                                }
                            }
                    }

                    // Фоллбек — ініціал
                    Text {
                        anchors.centerIn: parent
                        visible: model.avatarUrl === ""
                        text: model.avatarInitial
                        color: "#ffffff"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Black
                    }
                }

                // Ім'я + email
                Column {
                    x: 60
                    y: 10
                    width: parent.width - 88
                    spacing: 0

                    Text {
                        width: parent.width
                        height: 20
                        text: model.accountName
                        color: "#101828"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    Text {
                        width: parent.width
                        height: 16
                        text: model.accountEmail
                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                }

                // Галочка активного акаунту
                Shape {
                    x: parent.width - 28
                    y: 20
                    width: 16
                    height: 16
                    visible: model.isActive

                    ShapePath {
                        fillColor: "transparent"
                        strokeColor: "#2b7fff"
                        strokeWidth: 1.33
                        PathSvg { path: "M 10.67 0 L 3.33 7.33 L 0 4" }
                    }
                }

                MouseArea {
                    id: delegateMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        currentUser.authorize
                        (
                            accountName,
                            accountEmail,
                            avatarUrl
                        )
                    }
                }
            }
        }
    }

    // Розділювач перед кнопкою
    Rectangle {
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
    Rectangle {
        id: addButton
        anchors.top: divider.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 72
        color: "transparent"

        Rectangle {
            x: 8
            y: 8
            width: parent.width - 16
            height: 56
            radius: 10
            color: addMouseArea.containsMouse ? "#f9fafb" : "transparent"

            // Іконка аватарки з плюсом
            Rectangle {
                x: 12
                y: 10
                width: 36
                height: 36
                radius: 18
                color: "transparent"
                border.color: "#e5e7eb"
                border.width: 1

                Image {
                    source: "qrc:/pngs/assets/ic_plus.svg"
                    width: 12
                    height: 12
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Text {
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

            MouseArea {
                id: addMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                // onClicked: AccountController.startOAuth()
                onClicked: loginPopup.open()
            }
        }
    }
    AddAccountPopup {
        id: loginPopup
    }

}


