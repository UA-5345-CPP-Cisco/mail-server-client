import QtQuick
import QtQuick.Shapes



Rectangle {
    id: settingsQML

    implicitHeight: 313
    implicitWidth: 382

    clip: true
    color: "#ffffff"
    radius: 14

    function closeGeneralSettingsWindow() {
        generalSettingsLoader.active = false
        generalSettingsLoader.source = ""
    }

    Rectangle {
        id: container

        x: 16
        y: 16

        height: 24
        width: 65

        color: "transparent"

        Text {
            id: settings

            height: 24
            width: 66

            color: "#101828"
            font.family: "Segoe UI"
            font.pixelSize: 16
            font.weight: Font.Black
            horizontalAlignment: Text.AlignLeft
            lineHeight: 24
            lineHeightMode: Text.FixedHeight
            text: "General"
            textFormat: Text.PlainText
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: buttonToCloseGeneralWindow

        x: 348.40
        y: 19

        height: 18
        width: 18

        color: "transparent"

        Rectangle {
            id: sVG

            height: 18
            width: 18

            clip: true
            color: "transparent"

            MouseArea
            {
                id: clickAreaCloseSettingsWindow
                anchors.fill: parent

                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked:
                {
                    closeGeneralSettingsWindow()
                }
            }
            scale: clickAreaCloseSettingsWindow.containsMouse ? 1.5 : 1.0

            Behavior on scale {
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            Shape {
                id: leftArrow
                x: 2.00
                y: 4.50
                height: 9
                width: 6

                ShapePath {
                    id: leftArrowShapePath
                    fillColor: "#00000000"
                    strokeColor: "#6a7282"
                    strokeWidth: 1.50
                    capStyle: ShapePath.RoundCap
                    joinStyle: ShapePath.RoundJoin

                    PathSvg {
                        // M 6 0
                        // L 0 4.5
                        // L 6 9
                        path: "M 6 0 L 0 4.5 L 6 9"
                    }
                }
            }}
    }

    Rectangle {
        id: container_1

        x: 11
        y: 284

        height: 16
        width: 350.40

        color: "transparent"

        Text {
            id: mail_App_v1_0

            x: 138.70

            height: 16
            width: 74

            color: "#6a7282"
            font.family: "Segoe UI"
            font.pixelSize: 12
            font.weight: Font.Normal
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 16
            lineHeightMode: Text.FixedHeight
            textFormat: Text.PlainText
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: container_2

        y: 64

        height: 220
        width: 382.40

        color: "transparent"

        Rectangle {
            id: container_3

            x: 16
            y: 16

            height: 60
            width: 350.40

            color: "transparent"
            radius: 10

            Rectangle {
                id: container_4

                x: 16
                y: 12

                height: 36
                width: 106

                color: "transparent"

                Rectangle {
                    id: sVG_1

                    y: 9

                    height: 18
                    width: 18

                    clip: true
                    color: "transparent"

                    Shape {
                        id: _vector_2

                        x: 6
                        y: 6

                        height: 6
                        width: 6

                        ShapePath {
                            id: _vector_2_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#4a5565"
                            strokeWidth: 1.50

                            PathSvg {
                                id: _vector_2_ShapePath0_PathSvg0

                                path: "M 6 3 C 6 4.656854331493378 4.656854331493378 6 3 6 C 1.3431456685066223 6 0 4.656854331493378 0 3 C 0 1.3431456685066223 1.3431456685066223 0 3 0 C 4.656854331493378 0 6 1.3431456685066223 6 3 Z"
                            }
                        }
                    }
                    Shape {
                        id: _vector_3

                        x: 9
                        y: 1.50

                        height: 1.50
                        width: 0

                        ShapePath {
                            id: _vector_3_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#4a5565"
                            strokeWidth: 1.50

                            PathSvg {
                                id: _vector_3_ShapePath0_PathSvg0

                                path: "M 0 0 L 0 1.5"
                            }
                        }
                    }
                    Shape {
                        id: _vector_4

                        x: 9
                        y: 15

                        height: 1.50
                        width: 0

                        ShapePath {
                            id: _vector_4_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#4a5565"
                            strokeWidth: 1.50

                            PathSvg {
                                id: _vector_4_ShapePath0_PathSvg0

                                path: "M 0 0 L 0 1.5"
                            }
                        }
                    }
                    Shape {
                        id: _vector_5

                        x: 3.70
                        y: 3.70

                        height: 1.06
                        width: 1.06

                        ShapePath {
                            id: _vector_5_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#4a5565"
                            strokeWidth: 1.50

                            PathSvg {
                                id: _vector_5_ShapePath0_PathSvg0

                                path: "M 0 0 L 1.0575002431869507 1.0575002431869507"
                            }
                        }
                    }
                    Shape {
                        id: _vector_6

                        x: 13.24
                        y: 13.24

                        height: 1.06
                        width: 1.06

                        ShapePath {
                            id: _vector_6_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#4a5565"
                            strokeWidth: 1.50

                            PathSvg {
                                id: _vector_6_ShapePath0_PathSvg0

                                path: "M 0 0 L 1.057499885559082 1.057499885559082"
                            }
                        }
                    }
                    Shape {
                        id: _vector_7

                        x: 1.50
                        y: 9

                        height: 0
                        width: 1.50

                        ShapePath {
                            id: _vector_7_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#4a5565"
                            strokeWidth: 1.50

                            PathSvg {
                                id: _vector_7_ShapePath0_PathSvg0

                                path: "M 0 0 L 1.5 0"
                            }
                        }
                    }
                    Shape {
                        id: _vector_8

                        x: 15
                        y: 9

                        height: 0
                        width: 1.50

                        ShapePath {
                            id: _vector_8_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#4a5565"
                            strokeWidth: 1.50

                            PathSvg {
                                id: _vector_8_ShapePath0_PathSvg0

                                path: "M 0 0 L 1.5 0"
                            }
                        }
                    }
                    Shape {
                        id: _vector_9

                        x: 3.70
                        y: 13.24

                        height: 1.06
                        width: 1.06

                        ShapePath {
                            id: _vector_9_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#4a5565"
                            strokeWidth: 1.50

                            PathSvg {
                                id: _vector_9_ShapePath0_PathSvg0

                                path: "M 1.0575002431869507 0 L 0 1.057499885559082"
                            }
                        }
                    }
                    Shape {
                        id: _vector_10

                        x: 13.24
                        y: 3.70

                        height: 1.06
                        width: 1.06

                        ShapePath {
                            id: _vector_10_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#4a5565"
                            strokeWidth: 1.50

                            PathSvg {
                                id: _vector_10_ShapePath0_PathSvg0

                                path: "M 1.057499885559082 0 L 0 1.0575002431869507"
                            }
                        }
                    }
                }
                Rectangle {
                    id: container_5

                    x: 30

                    height: 36
                    width: 76

                    color: "transparent"

                    Rectangle {
                        id: container_6

                        height: 20
                        width: 76

                        color: "transparent"

                        Text {
                            id: dark_Mode

                            height: 20
                            width: 70

                            color: "#101828"
                            font.family: "Segoe UI"
                            font.pixelSize: 14
                            font.weight: Font.Normal
                            horizontalAlignment: Text.AlignLeft
                            lineHeight: 20
                            lineHeightMode: Text.FixedHeight
                            text: "Dark Mode"
                            textFormat: Text.PlainText
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    Rectangle {
                        id: container_7

                        y: 20

                        height: 16
                        width: 76

                        color: "transparent"

                        Text {
                            id: currently_light

                            height: 16
                            width: 77

                            color: "#6a7282"
                            font.family: "Segoe UI"
                            font.pixelSize: 12
                            font.weight: Font.Normal
                            horizontalAlignment: Text.AlignLeft
                            lineHeight: 16
                            lineHeightMode: Text.FixedHeight
                            text: "Currently light"
                            textFormat: Text.PlainText
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
            Rectangle {
                id: buttonToSwitchToDarkMode

                x: 290.40
                y: 18

                height: 24
                width: 44

                color: "#d1d5dc"
                radius: 26843500

                Rectangle {
                    id: background

                    x: 4
                    y: 4

                    height: 16
                    width: 16

                    color: "#ffffff"
                    radius: 26843500

                    Image {
                        id: overlay_Shadow

                        x: -3
                        y: -2

                        source: Qt.resolvedUrl("assets/overlay_Shadow.png")
                    }
                }
            }
        }

        Rectangle {
            id: container_8
            x: 16
            y: 72
            width: 350.4
            height: 60
            color: "transparent"
            radius: 10
            Rectangle {
                id: container_9
                x: 16
                y: 12
                width: 106
                height: 36
                color: "transparent"
                Rectangle {
                    id: sVG_2
                    y: 9
                    width: 18
                    height: 18
                    color: "transparent"
                    Shape {
                        id: _vector_11
                        x: 6
                        y: 6
                        width: 6
                        height: 6
                        ShapePath {
                            id: _vector_2_ShapePath1
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_2_ShapePath0_PathSvg1
                                path: "M 6 3 C 6 4.656854331493378 4.656854331493378 6 3 6 C 1.3431456685066223 6 0 4.656854331493378 0 3 C 0 1.3431456685066223 1.3431456685066223 0 3 0 C 4.656854331493378 0 6 1.3431456685066223 6 3 Z"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_12
                        x: 9
                        y: 1.5
                        width: 0
                        height: 1.5
                        ShapePath {
                            id: _vector_3_ShapePath1
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_3_ShapePath0_PathSvg1
                                path: "M 0 0 L 0 1.5"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_13
                        x: 9
                        y: 15
                        width: 0
                        height: 1.5
                        ShapePath {
                            id: _vector_4_ShapePath1
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_4_ShapePath0_PathSvg1
                                path: "M 0 0 L 0 1.5"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_14
                        x: 3.7
                        y: 3.7
                        width: 1.06
                        height: 1.06
                        ShapePath {
                            id: _vector_5_ShapePath1
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_5_ShapePath0_PathSvg1
                                path: "M 0 0 L 1.0575002431869507 1.0575002431869507"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_15
                        x: 13.24
                        y: 13.24
                        width: 1.06
                        height: 1.06
                        ShapePath {
                            id: _vector_6_ShapePath1
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_6_ShapePath0_PathSvg1
                                path: "M 0 0 L 1.057499885559082 1.057499885559082"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_16
                        x: 1.5
                        y: 9
                        width: 1.5
                        height: 0
                        ShapePath {
                            id: _vector_7_ShapePath1
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_7_ShapePath0_PathSvg1
                                path: "M 0 0 L 1.5 0"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_17
                        x: 15
                        y: 9
                        width: 1.5
                        height: 0
                        ShapePath {
                            id: _vector_8_ShapePath1
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_8_ShapePath0_PathSvg1
                                path: "M 0 0 L 1.5 0"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_18
                        x: 3.7
                        y: 13.24
                        width: 1.06
                        height: 1.06
                        ShapePath {
                            id: _vector_9_ShapePath1
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_9_ShapePath0_PathSvg1
                                path: "M 1.0575002431869507 0 L 0 1.057499885559082"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_19
                        x: 13.24
                        y: 3.7
                        width: 1.06
                        height: 1.06
                        ShapePath {
                            id: _vector_10_ShapePath1
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_10_ShapePath0_PathSvg1
                                path: "M 1.057499885559082 0 L 0 1.0575002431869507"
                            }
                            fillColor: "#00000000"
                        }
                    }
                    clip: true
                }

                Rectangle {
                    id: container_10
                    x: 30
                    width: 76
                    height: 36
                    color: "transparent"
                    Rectangle {
                        id: container_11
                        width: 76
                        height: 20
                        color: "transparent"
                        Text {
                            id: dark_Mode1
                            width: 70
                            height: 20
                            color: "#101828"
                            text: "TestSet"
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            lineHeight: 20
                            lineHeightMode: Text.FixedHeight
                            textFormat: Text.PlainText
                            font.weight: Font.Normal
                            font.family: "Segoe UI"
                        }
                    }

                    Rectangle {
                        id: container_12
                        y: 20
                        width: 76
                        height: 16
                        color: "transparent"
                        Text {
                            id: currently_light1
                            width: 77
                            height: 16
                            color: "#6a7282"
                            text: "Currently light"
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            lineHeight: 16
                            lineHeightMode: Text.FixedHeight
                            textFormat: Text.PlainText
                            font.weight: Font.Normal
                            font.family: "Segoe UI"
                        }
                    }
                }
            }

            Rectangle {
                id: buttonToSwitchToDarkMode1
                x: 290.4
                y: 18
                width: 44
                height: 24
                color: "#d1d5dc"
                radius: 26843500
                Rectangle {
                    id: background1
                    x: 4
                    y: 4
                    width: 16
                    height: 16
                    color: "#ffffff"
                    radius: 26843500
                    Image {
                        id: overlay_Shadow1
                        x: -3
                        y: -2
                        source: Qt.resolvedUrl("assets/overlay_Shadow1.png")
                    }
                }
            }
        }

        Rectangle {
            id: container_13
            x: 16
            y: 131
            width: 350.4
            height: 60
            color: "transparent"
            radius: 10
            Rectangle {
                id: container_14
                x: 16
                y: 12
                width: 106
                height: 36
                color: "transparent"
                Rectangle {
                    id: sVG_3
                    y: 9
                    width: 18
                    height: 18
                    color: "transparent"
                    Shape {
                        id: _vector_20
                        x: 6
                        y: 6
                        width: 6
                        height: 6
                        ShapePath {
                            id: _vector_2_ShapePath2
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_2_ShapePath0_PathSvg2
                                path: "M 6 3 C 6 4.656854331493378 4.656854331493378 6 3 6 C 1.3431456685066223 6 0 4.656854331493378 0 3 C 0 1.3431456685066223 1.3431456685066223 0 3 0 C 4.656854331493378 0 6 1.3431456685066223 6 3 Z"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_21
                        x: 9
                        y: 1.5
                        width: 0
                        height: 1.5
                        ShapePath {
                            id: _vector_3_ShapePath2
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_3_ShapePath0_PathSvg2
                                path: "M 0 0 L 0 1.5"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_22
                        x: 9
                        y: 15
                        width: 0
                        height: 1.5
                        ShapePath {
                            id: _vector_4_ShapePath2
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_4_ShapePath0_PathSvg2
                                path: "M 0 0 L 0 1.5"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_23
                        x: 3.7
                        y: 3.7
                        width: 1.06
                        height: 1.06
                        ShapePath {
                            id: _vector_5_ShapePath2
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_5_ShapePath0_PathSvg2
                                path: "M 0 0 L 1.0575002431869507 1.0575002431869507"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_24
                        x: 13.24
                        y: 13.24
                        width: 1.06
                        height: 1.06
                        ShapePath {
                            id: _vector_6_ShapePath2
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_6_ShapePath0_PathSvg2
                                path: "M 0 0 L 1.057499885559082 1.057499885559082"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_25
                        x: 1.5
                        y: 9
                        width: 1.5
                        height: 0
                        ShapePath {
                            id: _vector_7_ShapePath2
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_7_ShapePath0_PathSvg2
                                path: "M 0 0 L 1.5 0"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_26
                        x: 15
                        y: 9
                        width: 1.5
                        height: 0
                        ShapePath {
                            id: _vector_8_ShapePath2
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_8_ShapePath0_PathSvg2
                                path: "M 0 0 L 1.5 0"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_27
                        x: 3.7
                        y: 13.24
                        width: 1.06
                        height: 1.06
                        ShapePath {
                            id: _vector_9_ShapePath2
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_9_ShapePath0_PathSvg2
                                path: "M 1.0575002431869507 0 L 0 1.057499885559082"
                            }
                            fillColor: "#00000000"
                        }
                    }

                    Shape {
                        id: _vector_28
                        x: 13.24
                        y: 3.7
                        width: 1.06
                        height: 1.06
                        ShapePath {
                            id: _vector_10_ShapePath2
                            strokeWidth: 1.5
                            strokeColor: "#4a5565"
                            PathSvg {
                                id: _vector_10_ShapePath0_PathSvg2
                                path: "M 1.057499885559082 0 L 0 1.0575002431869507"
                            }
                            fillColor: "#00000000"
                        }
                    }
                    clip: true
                }

                Rectangle {
                    id: container_15
                    x: 30
                    width: 76
                    height: 36
                    color: "transparent"
                    Rectangle {
                        id: container_16
                        width: 76
                        height: 20
                        color: "transparent"
                        Text {
                            id: dark_Mode2
                            width: 70
                            height: 20
                            color: "#101828"
                            text: "TestSser"
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            lineHeight: 20
                            lineHeightMode: Text.FixedHeight
                            textFormat: Text.PlainText
                            font.weight: Font.Normal
                            font.family: "Segoe UI"
                        }
                    }

                    Rectangle {
                        id: container_17
                        y: 20
                        width: 76
                        height: 16
                        color: "transparent"
                        Text {
                            id: currently_light2
                            width: 77
                            height: 16
                            color: "#6a7282"
                            text: "Currently light"
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            lineHeight: 16
                            lineHeightMode: Text.FixedHeight
                            textFormat: Text.PlainText
                            font.weight: Font.Normal
                            font.family: "Segoe UI"
                        }
                    }
                }
            }

            Rectangle {
                id: buttonToSwitchToDarkMode2
                x: 290.4
                y: 18
                width: 44
                height: 24
                color: "#d1d5dc"
                radius: 26843500
                Rectangle {
                    id: background2
                    x: 4
                    y: 4
                    width: 16
                    height: 16
                    color: "#ffffff"
                    radius: 26843500
                    Image {
                        id: overlay_Shadow2
                        x: -3
                        y: -2
                        source: Qt.resolvedUrl("assets/overlay_Shadow2.png")
                    }
                }
            }
        }
    }
}