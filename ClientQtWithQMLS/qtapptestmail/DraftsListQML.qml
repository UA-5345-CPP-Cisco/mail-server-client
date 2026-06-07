import QtQuick
import QtQuick.Shapes

Image {
    id: draftsListQML

    source: Qt.resolvedUrl("assets/draftsListQML.png")

    Image {
        id: horizontalBorder

        source: Qt.resolvedUrl("assets/horizontalBorder_4.png")

        Rectangle {
            id: container

            x: 16
            y: 16

            height: 38.20
            width: 287.20

            color: "transparent"

            Rectangle {
                id: input

                height: 38.20
                width: 287.20

                border.color: "#d1d5dc"
                border.width: 1
                clip: true
                color: "#ffffff"
                radius: 10

                Rectangle {
                    id: container_1

                    x: 36.80
                    y: 9.60

                    height: 19
                    width: 233.60

                    clip: true
                    color: "transparent"

                    Text {
                        id: search_mail

                        height: 19
                        width: 235

                        color: "#99a1af"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        text: "Search mail"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }
            }
            Rectangle {
                id: sVG

                x: 12
                y: 11.10

                height: 16
                width: 16

                clip: true
                color: "transparent"

                Shape {
                    id: _vector

                    x: 2
                    y: 2

                    height: 10.67
                    width: 10.67

                    ShapePath {
                        id: _vector_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_ShapePath0_PathSvg0

                            path: "M 10.666666984558105 5.333333492279053 C 10.666666984558105 8.278852391606137 8.278852391606137 10.666666984558105 5.333333492279053 10.666666984558105 C 2.387814592951969 10.666666984558105 0 8.278852391606137 0 5.333333492279053 C 0 2.387814592951969 2.387814592951969 0 5.333333492279053 0 C 8.278852391606137 0 10.666666984558105 2.387814592951969 10.666666984558105 5.333333492279053 Z"
                        }
                    }
                }
                Shape {
                    id: _vector_1

                    x: 11.13
                    y: 11.13

                    height: 2.87
                    width: 2.87

                    ShapePath {
                        id: _vector_1_ShapePath0

                        fillColor: "#00000000"
                        strokeColor: "#6a7282"
                        strokeWidth: 1.33

                        PathSvg {
                            id: _vector_1_ShapePath0_PathSvg0

                            path: "M 2.866666316986084 2.866666316986084 L 0 0"
                        }
                    }
                }
            }
        }
    }
    Image {
        id: inboxSeparator

        y: 71

        source: Qt.resolvedUrl("assets/inboxSeparator_1.png")

        Rectangle {
            id: heading_3

            x: 16
            y: 8

            height: 18
            width: 289

            color: "transparent"

            Rectangle {
                id: frame_3

                height: 18
                width: 250

                color: "transparent"

                Text {
                    id: drafts

                    height: 16
                    width: 53

                    color: "#6a7282"
                    font.capitalization: Font.AllUppercase
                    font.family: "Segoe UI"
                    font.letterSpacing: 0.60
                    font.pixelSize: 12
                    font.weight: Font.Black
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: "Drafts"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle {
                    id: frame_5

                    x: 191

                    height: 18
                    width: 55

                    color: "transparent"

                    Text {
                        id: amount

                        y: 1

                        height: 16
                        width: 57

                        color: "#000000"
                        font.capitalization: Font.AllUppercase
                        font.family: "Segoe UI"
                        font.letterSpacing: 0.60
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 16
                        lineHeightMode: Text.FixedHeight
                        text: "Amount"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            Rectangle {
                id: frame_4

                x: 250

                height: 18
                width: 12

                color: "transparent"

                Rectangle {
                    id: button_margin

                    y: 1

                    height: 16
                    width: 15

                    color: "transparent"

                    Rectangle {
                        id: buttonToFavouriteHolder

                        y: 2

                        height: 15
                        width: 15

                        clip: true
                        color: "transparent"

                        Shape {
                            id: buttonToMoveBack

                            x: 4
                            y: 1

                            height: 12
                            width: 6

                            rotation: -180

                            ShapePath {
                                id: buttonToMoveBack_ShapePath0

                                fillColor: "#00000000"
                                strokeColor: "#6a7282"
                                strokeWidth: 2

                                PathSvg {
                                    id: buttonToMoveBack_ShapePath0_PathSvg0

                                    path: "M 0 12 L 6 6 L 0 0"
                                }
                            }
                        }
                    }
                }
                Rectangle {
                    id: button_margin_1

                    x: 35

                    height: 18
                    width: 15

                    color: "transparent"

                    Rectangle {
                        id: buttonToMoveForward

                        y: 2

                        height: 15
                        width: 15

                        clip: true
                        color: "transparent"

                        Shape {
                            id: buttonToMoveForvard

                            x: 5
                            y: 2

                            height: 12
                            width: 6

                            ShapePath {
                                id: buttonToMoveForvard_ShapePath0

                                fillColor: "#00000000"
                                strokeColor: "#6a7282"
                                strokeWidth: 2

                                PathSvg {
                                    id: buttonToMoveForvard_ShapePath0_PathSvg0

                                    path: "M 0 12 L 6 6 L 0 0"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Rectangle {
        id: container_2

        y: 105.80

        height: 547.10
        width: 319.20

        clip: true
        color: "transparent"

        Image {
            id: overlay_HorizontalBorder

            source: Qt.resolvedUrl("assets/overlay_HorizontalBorder.png")

            Rectangle {
                id: container_3

                x: 16
                y: 16

                height: 84
                width: 287.20

                color: "transparent"

                Rectangle {
                    id: container_4

                    height: 20
                    width: 287.20

                    color: "transparent"

                    Rectangle {
                        id: container_5

                        height: 20
                        width: 122

                        clip: true
                        color: "transparent"

                        Text {
                            id: ceo_company_com

                            height: 20
                            width: 123

                            color: "#101828"
                            font.family: "Segoe UI"
                            font.pixelSize: 14
                            font.weight: Font.Normal
                            horizontalAlignment: Text.AlignLeft
                            lineHeight: 20
                            lineHeightMode: Text.FixedHeight
                            text: "ceo@company.com"
                            textFormat: Text.PlainText
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    Rectangle {
                        id: _margin

                        x: 251.20
                        y: 2

                        height: 16
                        width: 36

                        color: "transparent"

                        Text {
                            id: jun_4

                            x: 8

                            height: 16
                            width: 29

                            color: "#6a7282"
                            font.family: "Segoe UI"
                            font.pixelSize: 12
                            font.weight: Font.Normal
                            horizontalAlignment: Text.AlignLeft
                            lineHeight: 16
                            lineHeightMode: Text.FixedHeight
                            text: "Jun 4"
                            textFormat: Text.PlainText
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
                Rectangle {
                    id: container_6

                    y: 22

                    height: 20
                    width: 287.20

                    clip: true
                    color: "transparent"

                    Text {
                        id: quarterly_Report_Summary

                        height: 20
                        width: 289

                        color: "#101828"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                        text: "Quarterly Report Summary"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }
                Rectangle {
                    id: container_7

                    y: 44

                    height: 19.59
                    width: 287.20

                    clip: true
                    color: "transparent"

                    Text {
                        id: draft_of_the_quarterly_summary_for_leadership_re

                        height: 16
                        width: 289

                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 16
                        lineHeightMode: Text.FixedHeight
                        text: "Draft of the quarterly summary for leadership review..."
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }
                Text {
                    id: draft

                    y: 65.59

                    height: 16
                    width: 28

                    color: "#fe9a00"
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: "Draft"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
    Rectangle {
        id: container_8

        y: 652.90

        height: 547.10
        width: 319.20

        clip: true
        color: "transparent"

        Image {
            id: overlay_HorizontalBorder_1

            source: Qt.resolvedUrl("assets/overlay_HorizontalBorder_1.png")

            Rectangle {
                id: container_9

                x: 16
                y: 16

                height: 84
                width: 287.20

                color: "transparent"

                Rectangle {
                    id: container_10

                    height: 20
                    width: 287.20

                    color: "transparent"

                    Rectangle {
                        id: container_11

                        height: 20
                        width: 122

                        clip: true
                        color: "transparent"

                        Text {
                            id: ceo_company_com_1

                            height: 20
                            width: 123

                            color: "#101828"
                            font.family: "Segoe UI"
                            font.pixelSize: 14
                            font.weight: Font.Normal
                            horizontalAlignment: Text.AlignLeft
                            lineHeight: 20
                            lineHeightMode: Text.FixedHeight
                            text: "ceo@company.com"
                            textFormat: Text.PlainText
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    Rectangle {
                        id: _margin_1

                        x: 251.20
                        y: 2

                        height: 16
                        width: 36

                        color: "transparent"

                        Text {
                            id: jun_5

                            x: 8

                            height: 16
                            width: 29

                            color: "#6a7282"
                            font.family: "Segoe UI"
                            font.pixelSize: 12
                            font.weight: Font.Normal
                            horizontalAlignment: Text.AlignLeft
                            lineHeight: 16
                            lineHeightMode: Text.FixedHeight
                            text: "Jun 4"
                            textFormat: Text.PlainText
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
                Rectangle {
                    id: container_12

                    y: 22

                    height: 20
                    width: 287.20

                    clip: true
                    color: "transparent"

                    Text {
                        id: quarterly_Report_Summary_1

                        height: 20
                        width: 289

                        color: "#101828"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                        text: "Quarterly Report Summary"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }
                Rectangle {
                    id: container_13

                    y: 44

                    height: 19.59
                    width: 287.20

                    clip: true
                    color: "transparent"

                    Text {
                        id: draft_of_the_quarterly_summary_for_leadership_re_1

                        height: 16
                        width: 289

                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 16
                        lineHeightMode: Text.FixedHeight
                        text: "Draft of the quarterly summary for leadership review..."
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }
                Text {
                    id: draft_1

                    y: 65.59

                    height: 16
                    width: 28

                    color: "#fe9a00"
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: "Draft"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}