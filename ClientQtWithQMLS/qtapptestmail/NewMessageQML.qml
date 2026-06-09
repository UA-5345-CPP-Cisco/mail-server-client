import QtQuick
import QtQuick.Shapes

Rectangle {
    id: newMessageQML

    implicitHeight: 398
    implicitWidth: 420
    border.color: "#e5e7eb"

    clip: true
    color: "#fcf3e6"
    radius: 14

    Rectangle {
        id: textarea

        y: 119

        height: 236.40
        width: 420
        radius: 14

        clip: true
        color: "#fcf3e6"

        Rectangle {
            id: container

            x: 12
            y: 12

            height: 20
            width: 396
            topLeftRadius: 14

            color: "transparent"

            Text {
                id: write_your_message_

                height: 20
                width: 397

                color: "#99a1af"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                text: "Write your message..."
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }
        }
    }

    Rectangle {
        id: background

        height: 38
        width: 420
        topLeftRadius: 14
        topRightRadius: 14

        color: "#1e2939"
        MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.OpenHandCursor // Курсор у вигляді відкритої руки

                    // Вказуємо, що тягнемо головний компонент (id: newMessageQML)
                    drag.target: newMessageQML
                    drag.axis: Drag.XAndYAxis

                    // Змінюємо курсор при затисканні
                    onPressed: cursorShape = Qt.ClosedHandCursor
                    onReleased: cursorShape = Qt.OpenHandCursor
                }

        Rectangle {
            id: container_1

            x: 12
            y: 9

            height: 20
            width: 306

            clip: true
            color: "transparent"

            Text {
                id: new_Message

                height: 20
                width: 307

                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                text: "New Message"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }
        }

        Rectangle {
            id: buttonToMinimizeWindow

            x: 326
            y: 8

            height: 22
            width: 22

            color: "transparent"
            radius: 4

            Rectangle {
                id: sVG

                x: 4
                y: 4

                height: 14
                width: 14

                clip: true
                color: "transparent"

                Shape {
                    id: _vector

                    x: 2.92
                    y: 7

                    height: 0
                    width: 8.17

                    ShapePath {
                        id: _vector_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_ShapePath0_PathSvg0

                            path: "M 0 0 L 8.166666030883789 0"
                        }
                    }
                }
            }
        }

        Rectangle {
            id: buttonToMaximizeWindow

            x: 356
            y: 8

            height: 22
            width: 22

            color: "transparent"
            radius: 4

            Rectangle {
                id: sVG_1

                x: 4
                y: 4

                height: 14
                width: 14

                clip: true
                color: "transparent"

                Shape {
                    id: _vector_1

                    x: 8.75
                    y: 1.75

                    height: 3.50
                    width: 3.50

                    ShapePath {
                        id: _vector_1_ShapePath0

                        fillColor: "#00000000"
                        fillRule: ShapePath.WindingFill
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_1_ShapePath0_PathSvg0

                            path: "M 0 0 L 3.5 0 L 3.5 3.5"
                        }
                    }
                }
                Shape {
                    id: _vector_2

                    x: 1.75
                    y: 8.75

                    height: 3.50
                    width: 3.50

                    ShapePath {
                        id: _vector_2_ShapePath0

                        fillColor: "#00000000"
                        fillRule: ShapePath.WindingFill
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_2_ShapePath0_PathSvg0

                            path: "M 3.5 3.5 L 0 3.5 L 0 0"
                        }
                    }
                }
                Shape {
                    id: _vector_3

                    x: 8.17
                    y: 1.75

                    height: 4.08
                    width: 4.08

                    ShapePath {
                        id: _vector_3_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_3_ShapePath0_PathSvg0

                            path: "M 4.0833330154418945 0 L 0 4.0833330154418945"
                        }
                    }
                }
                Shape {
                    id: _vector_4

                    x: 1.75
                    y: 8.17

                    height: 4.08
                    width: 4.08

                    ShapePath {
                        id: _vector_4_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_4_ShapePath0_PathSvg0

                            path: "M 0 4.0833330154418945 L 4.0833330154418945 0"
                        }
                    }
                }
            }
        }

        Rectangle {
            id: buttonToCloseWindow

            x: 386
            y: 8

            height: 22
            width: 22

            color: clickAreaCloseMessageBox.pressed ? "#ffdede" : "transparent"
            radius: 4
            MouseArea {
                id: clickAreaCloseMessageBox
                    cursorShape: Qt.PointingHandCursor
                    anchors.fill: parent
                    onClicked: {
                        closeMessageWindow()
                    }
                }
            Rectangle {
                id: sVG_2

                x: 4
                y: 4

                height: 14
                width: 14

                clip: true
                color: "transparent"

                Shape {
                    id: _vector_5

                    x: 3.50
                    y: 3.50

                    height: 7
                    width: 7

                    ShapePath {
                        id: _vector_5_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_5_ShapePath0_PathSvg0

                            path: "M 7 0 L 0 7"
                        }
                    }
                }
                Shape {
                    id: _vector_6

                    x: 3.50
                    y: 3.50

                    height: 7
                    width: 7

                    ShapePath {
                        id: _vector_6_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#ffffff"
                        strokeWidth: 1.17

                        PathSvg {
                            id: _vector_6_ShapePath0_PathSvg0

                            path: "M 0 0 L 7 7"
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: input_1_bg

        y: 38

        width: 420
        height: 36

        clip: true
        color: "#fcf3e6"
        border.color: "#e5e7eb"
        border.width: 1

        Rectangle {
            id: container_3

            x: 12
            y: 8.80

            height: 19
            width: 396

            clip: true
            color: "transparent"

            Text {
                id: subject

                height: 19
                width: 397

                color: "#99a1af"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                text: "To"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }
        }
    }

    Rectangle {
        id: input_2_bg

        y: 72

        width: 420
        height: 36

        clip: true
        color: "#fcf3e6"
        border.color: "#e5e7eb"
        border.width: 1

        Rectangle {
            id: container_4

            x: 12
            y: 8.80

            height: 19
            width: 396

            clip: true
            color: "transparent"

            Text {
                id: subject_1

                height: 19
                width: 397

                color: "#99a1af"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                text: "Subject"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }
        }
    }

    Rectangle {
        id: horizontalBorder

        y: 340

        Rectangle {
            id: buttonToSend

            x: 12
            y: 8.80

            height: 32
            width: 63
            bottomLeftRadius: 14
            bottomRightRadius: 14

            color: "#155dfc"
            radius: 26843500

            Text {
                id: send

                x: 16
                y: 6

                height: 20
                width: 32

                color: "#ffffff"
                font.family: "Segoe UI"
                font.pixelSize: 14
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignHCenter
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                text: "Send"
                textFormat: Text.PlainText
                verticalAlignment: Text.AlignVCenter
            }
        }
        Rectangle {
            id: buttonToDelete

            x: 83
            y: 10.80

            height: 28
            width: 28

            color: "transparent"
            radius: 4

            Rectangle {
                id: sVG_3

                x: 6
                y: 6

                height: 16
                width: 16

                clip: true
                color: "transparent"

                Shape {
                    id: _vector_7

                    x: 2
                    y: 4

                    height: 0
                    width: 12

                    ShapePath {
                        id: _vector_7_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_7_ShapePath0_PathSvg0

                            path: "M 0 0 L 12 0"
                        }
                    }
                }
                Shape {
                    id: _vector_8

                    x: 3.33
                    y: 4

                    height: 10.67
                    width: 9.33

                    ShapePath {
                        id: _vector_8_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_8_ShapePath0_PathSvg0

                            path: "M 9.333333969116211 0 L 9.333333969116211 9.333333611488342 C 9.333333969116211 10.000000298023224 8.666667257036481 10.666666984558105 8.000000544956752 10.666666984558105 L 1.3333334241594588 10.666666984558105 C 0.6666667120797294 10.666666984558105 0 10.000000298023224 0 9.333333611488342 L 0 0"
                        }
                    }
                }
                Shape {
                    id: _vector_9

                    x: 5.33
                    y: 1.33

                    height: 2.67
                    width: 5.33

                    ShapePath {
                        id: _vector_9_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_9_ShapePath0_PathSvg0

                            path: "M 0 2.6666667461395264 L 0 1.3333333730697632 C 0 0.6666666865348816 0.6666666865348816 0 1.3333333730697632 0 L 4.0000001192092896 0 C 4.666666805744171 0 5.333333492279053 0.6666666865348816 5.333333492279053 1.3333333730697632 L 5.333333492279053 2.6666667461395264"
                        }
                    }
                }
                Shape {
                    id: _vector_10

                    x: 6.67
                    y: 7.33

                    height: 4
                    width: 0

                    ShapePath {
                        id: _vector_10_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_10_ShapePath0_PathSvg0

                            path: "M 0 0 L 0 4"
                        }
                    }
                }
                Shape {
                    id: _vector_11

                    x: 9.33
                    y: 7.33

                    height: 4
                    width: 0

                    ShapePath {
                        id: _vector_11_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_11_ShapePath0_PathSvg0

                            path: "M 0 0 L 0 4"
                        }
                    }
                }
            }
        }
    }
}