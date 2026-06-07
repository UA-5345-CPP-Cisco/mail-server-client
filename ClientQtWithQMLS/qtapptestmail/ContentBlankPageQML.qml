import QtQuick
import QtQuick.Shapes

Rectangle {
    id: contentBlankPageQML

    height: 1200
    width: 1344

    clip: true
    color: "#ffffff"

    Rectangle {
        id: background

        height: 1200
        width: 1344

        color: "#ffffff"

        Rectangle {
            id: container

            height: 1200
            width: 1344

            color: "transparent"

            Rectangle {
                id: container_1

                x: 601.50
                y: 560

                height: 80
                width: 141

                color: "transparent"

                Rectangle {
                    id: sVG

                    x: 46.50

                    height: 48
                    width: 48

                    clip: true
                    color: "transparent"
                    opacity: 0.30

                    Shape {
                        id: _vector

                        x: 4
                        y: 8

                        height: 32
                        width: 40

                        ShapePath {
                            id: _vector_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#6a7282"
                            strokeWidth: 4

                            PathSvg {
                                id: _vector_ShapePath0_PathSvg0

                                path: "M 4 0 L 36 0 C 38.20913910865784 0 40 1.790860891342163 40 4 L 40 28 C 40 30.209139108657837 38.20913910865784 32 36 32 L 4 32 C 1.790860891342163 32 0 30.209139108657837 0 28 L 0 4 C 0 1.790860891342163 1.790860891342163 0 4 0 Z"
                            }
                        }
                    }
                    Shape {
                        id: _vector_1

                        x: 4
                        y: 14

                        height: 11.99
                        width: 40

                        ShapePath {
                            id: _vector_1_ShapePath0

                            fillColor: "#00000000"
                            strokeColor: "#6a7282"
                            strokeWidth: 4

                            PathSvg {
                                id: _vector_1_ShapePath0_PathSvg0

                                path: "M 40 0 L 22.059999465942383 11.399999618530273 C 21.442542791366577 11.786851555109024 20.728633761405945 11.992021560668949 20 11.992021560668945 C 19.271366238594055 11.992021560668949 18.557457208633423 11.786851555109024 17.940000534057617 11.399999618530273 L 0 0"
                            }
                        }
                    }
                }
                Rectangle {
                    id: container_2

                    y: 60

                    height: 20
                    width: 141

                    color: "transparent"

                    Text {
                        id: select_an_email_to_read

                        height: 20
                        width: 142

                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignHCenter
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                        text: "Select an email to read"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}