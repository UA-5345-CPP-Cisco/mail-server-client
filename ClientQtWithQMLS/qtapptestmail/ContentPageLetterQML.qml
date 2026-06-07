import QtQuick
import QtQuick.Shapes

Rectangle {
    id: contentPageLetterQML

    height: 1200
    width: 1345

    color: "#ffffff"

    Rectangle {
        id: container

        height: 1200
        width: 1345

        color: "transparent"

        Image {
            id: horizontalBorder

            source: Qt.resolvedUrl("assets/horizontalBorder_2.png")

            Rectangle {
                id: container_1

                x: 24
                y: 24

                height: 76
                width: 1297

                color: "transparent"

                Rectangle {
                    id: container_2

                    height: 76
                    width: 1166

                    color: "transparent"

                    Rectangle {
                        id: themeHolder

                        height: 28
                        width: 1166

                        color: "transparent"

                        Text {
                            id: themeHolder_1

                            height: 28
                            width: 1167

                            color: "#101828"
                            font.family: "Segoe UI"
                            font.pixelSize: 20
                            font.weight: Font.Black
                            horizontalAlignment: Text.AlignLeft
                            lineHeight: 28
                            lineHeightMode: Text.FixedHeight
                            text: "ThemeHolder"
                            textFormat: Text.PlainText
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                        }
                    }
                    Rectangle {
                        id: container_3

                        y: 40

                        height: 36
                        width: 1166

                        color: "transparent"

                        Rectangle {
                            id: background

                            height: 36
                            width: 36

                            color: "#2b7fff"
                            radius: 26843500

                            Text {
                                id: s

                                x: 13.50
                                y: 8

                                height: 20
                                width: 10

                                color: "#ffffff"
                                font.family: "Segoe UI"
                                font.pixelSize: 14
                                font.weight: Font.Black
                                horizontalAlignment: Text.AlignHCenter
                                lineHeight: 20
                                lineHeightMode: Text.FixedHeight
                                text: "S"
                                textFormat: Text.PlainText
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                        Rectangle {
                            id: container_4

                            x: 48

                            height: 36
                            width: 79

                            color: "transparent"

                            Rectangle {
                                id: nameHolder

                                height: 20
                                width: 79

                                color: "transparent"

                                Text {
                                    id: nameHolder_1

                                    height: 20
                                    width: 80

                                    color: "#101828"
                                    font.family: "Segoe UI"
                                    font.pixelSize: 14
                                    font.weight: Font.Normal
                                    horizontalAlignment: Text.AlignLeft
                                    lineHeight: 20
                                    lineHeightMode: Text.FixedHeight
                                    text: "NameHolder"
                                    textFormat: Text.PlainText
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                            Rectangle {
                                id: container_5

                                y: 20

                                height: 16
                                width: 79

                                color: "transparent"

                                Text {
                                    id: to_me

                                    height: 16
                                    width: 33

                                    color: "#6a7282"
                                    font.family: "Segoe UI"
                                    font.pixelSize: 12
                                    font.weight: Font.Normal
                                    horizontalAlignment: Text.AlignLeft
                                    lineHeight: 16
                                    lineHeightMode: Text.FixedHeight
                                    text: "to me"
                                    textFormat: Text.PlainText
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }
                Rectangle {
                    id: container_6

                    x: 1166

                    height: 34
                    width: 131

                    color: "transparent"

                    Rectangle {
                        id: timeHolder

                        y: 9

                        height: 16
                        width: 55

                        color: "transparent"

                        Text {
                            id: aM

                            height: 16
                            width: 52

                            color: "#6a7282"
                            font.family: "Segoe UI"
                            font.pixelSize: 12
                            font.weight: Font.Normal
                            horizontalAlignment: Text.AlignLeft
                            lineHeight: 16
                            lineHeightMode: Text.FixedHeight
                            text: "10:30 AM"
                            textFormat: Text.PlainText
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    Rectangle {
                        id: buttonToFavourite

                        x: 59

                        height: 34
                        width: 34

                        color: "transparent"
                        radius: 10

                        Rectangle {
                            id: sVG

                            x: 8
                            y: 8

                            height: 18
                            width: 18

                            clip: true
                            color: "transparent"

                            Shape {
                                id: _vector

                                x: 1.50
                                y: 1.50

                                height: 14.30
                                width: 15

                                ShapePath {
                                    id: _vector_ShapePath0

                                    fillColor: "#00000000"
                                    fillRule: ShapePath.WindingFill
                                    strokeColor: "#6a7282"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_ShapePath0_PathSvg0

                                        path: "M 7.145099628590124 0.22117031357864084 C 7.177967198179905 0.15476591842221038 7.228740407823365 0.09886951049691106 7.291689150843043 0.05978912314408925 C 7.354637893862721 0.020708735791267426 7.427256598597553 3.3306691849060214e-16 7.50134992601629 0 C 7.575443253435027 0 7.648061958169858 0.020708735791267426 7.711010701189537 0.05978912314408925 C 7.773959444209215 0.09886951049691106 7.824732653852674 0.15476591842221038 7.857600223442455 0.22117031357864084 L 9.590099877968958 3.7304203561757334 C 9.704232893146534 3.961395842763046 9.872709521053459 4.161226623642411 10.08106859779866 4.312760496857739 C 10.289427674543862 4.464294370073067 10.531445256845027 4.563005090997035 10.786349401009149 4.600420270736966 L 14.660850514079058 5.167420678737504 C 14.734264465872405 5.17805737917652 14.803237288480888 5.209024301219815 14.859967704093556 5.256819781067672 C 14.916698119706224 5.304615260915529 14.958922173410514 5.367331730312252 14.981864451742615 5.43787533291019 C 15.004806730074716 5.5084189355081286 15.007552595540526 5.5839752292941345 14.989789485563858 5.655997584062697 C 14.97202637558719 5.72801993883126 14.934463793148359 5.79363431164776 14.881350990295712 5.845420197799128 L 12.079350855454189 8.573921012594717 C 11.894562473487543 8.753996786979046 11.756303630066716 8.976282016651247 11.676476611309816 9.22164232477541 C 11.596649592552916 9.467002632899572 11.577645174365488 9.72808741424947 11.621100079855998 9.982421254097773 L 12.282600793250197 13.837420924843633 C 12.295559873944717 13.910799695107986 12.287633095290575 13.986343460742301 12.259724053021078 14.055433741864684 C 12.23181501075158 14.124524022987067 12.18504759658016 14.18437800809403 12.124758152124569 14.228168008182003 C 12.064468707668977 14.271958008269976 11.993084048225574 14.297920209267646 11.918749472862086 14.303092479673811 C 11.844414897498597 14.308264750079976 11.770123142485806 14.292438993774644 11.704350128278575 14.257420538301448 L 8.240849995298612 12.436420855251978 C 8.012637765587662 12.316590458956252 7.758735001839338 12.253983429278925 7.500975131998029 12.253983429278922 C 7.24321526215672 12.253983429278922 6.989311783152636 12.316590458956252 6.761099553441686 12.436420855251978 L 3.298349723754005 14.257420538301448 C 3.232598217510996 14.29222893489226 3.1583978028939352 14.307886285485916 3.0841882017107065 14.30261182780232 C 3.009978600527478 14.297337370118724 2.9387385065666387 14.271342458044346 2.8785711249768458 14.227584359480906 C 2.818403743387053 14.183826260917465 2.7717244174328304 14.124061861594589 2.74384197373691 14.055087558076288 C 2.7159595300409896 13.986113254557987 2.70799327820959 13.910698458478105 2.720849719702544 13.837420924843633 L 3.3815997721765814 9.983170842135456 C 3.4252468914767302 9.728719700342326 3.4063376470088746 9.467469203173264 3.3265032633528198 9.221955606798792 C 3.246668879696765 8.97644201042432 3.1083061453767917 8.754036472497917 2.9233497118341503 8.573921012594717 L 0.12134978933418115 5.846170501092573 C 0.06779515399957634 5.794444858475274 0.029844675230427102 5.728717419369157 0.01182159912164715 5.656476090166906 C -0.0062014769871328035 5.584234760964654 -0.0035725278984115386 5.508383653282535 0.019408853410211525 5.437563481398329 C 0.04239023471883459 5.366743309514123 0.08480005794600372 5.303801551316447 0.1418073557689054 5.255907829972208 C 0.1988146535918071 5.208014108627969 0.2681273679726346 5.177094213253987 0.34184971793314306 5.16667037544406 L 4.215599790103269 4.600420270736966 C 4.4707923388008854 4.563294741640623 4.713141531553799 4.464713454069743 4.921787895245369 4.313160682456101 C 5.130434258936939 4.161607910842459 5.299127586987413 3.9616234729090314 5.413349562100143 3.7304203561757334 L 7.145099628590124 0.22117031357864084 Z"
                                    }
                                }
                            }
                        }
                    }
                    Rectangle {
                        id: button

                        x: 97

                        height: 34
                        width: 34

                        color: "transparent"
                        radius: 10

                        Rectangle {
                            id: sVG_1

                            x: 8
                            y: 8

                            height: 18
                            width: 18

                            clip: true
                            color: "transparent"

                            Shape {
                                id: _vector_1

                                x: 8.25
                                y: 8.25

                                height: 1.50
                                width: 1.50

                                ShapePath {
                                    id: _vector_1_ShapePath0

                                    fillColor: "#00000000"
                                    strokeColor: "#4a5565"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_1_ShapePath0_PathSvg0

                                        path: "M 1.5 0.75 C 1.5 1.1642135828733444 1.1642135828733444 1.5 0.75 1.5 C 0.3357864171266556 1.5 0 1.1642135828733444 0 0.75 C 0 0.3357864171266556 0.3357864171266556 0 0.75 0 C 1.1642135828733444 0 1.5 0.3357864171266556 1.5 0.75 Z"
                                    }
                                }
                            }
                            Shape {
                                id: _vector_2

                                x: 8.25
                                y: 3

                                height: 1.50
                                width: 1.50

                                ShapePath {
                                    id: _vector_2_ShapePath0

                                    fillColor: "#00000000"
                                    strokeColor: "#4a5565"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_2_ShapePath0_PathSvg0

                                        path: "M 1.5 0.75 C 1.5 1.1642135828733444 1.1642135828733444 1.5 0.75 1.5 C 0.3357864171266556 1.5 0 1.1642135828733444 0 0.75 C 0 0.3357864171266556 0.3357864171266556 0 0.75 0 C 1.1642135828733444 0 1.5 0.3357864171266556 1.5 0.75 Z"
                                    }
                                }
                            }
                            Shape {
                                id: _vector_3

                                x: 8.25
                                y: 13.50

                                height: 1.50
                                width: 1.50

                                ShapePath {
                                    id: _vector_3_ShapePath0

                                    fillColor: "#00000000"
                                    strokeColor: "#4a5565"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_3_ShapePath0_PathSvg0

                                        path: "M 1.5 0.75 C 1.5 1.1642135828733444 1.1642135828733444 1.5 0.75 1.5 C 0.3357864171266556 1.5 0 1.1642135828733444 0 0.75 C 0 0.3357864171266556 0.3357864171266556 0 0.75 0 C 1.1642135828733444 0 1.5 0.3357864171266556 1.5 0.75 Z"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            id: container_7

            y: 140.80

            height: 988.80
            width: 1345

            clip: true
            color: "transparent"

            Rectangle {
                id: contentHolder

                x: 24
                y: 23.30

                height: 295.60
                width: 1297

                color: "transparent"

                Text {
                    id: hi_team_

                    height: 23
                    width: 53

                    color: "#4a5565"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 22.75
                    lineHeightMode: Text.FixedHeight
                    text: "Hi team,"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: i_wanted_to_share_the_latest_updates_on_our_Q2_p

                    y: 45.40

                    height: 23
                    width: 952

                    color: "#4a5565"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 22.75
                    lineHeightMode: Text.FixedHeight
                    text: "I wanted to share the latest updates on our Q2 projects. We've made significant progress on the main deliverables and are on track to meet our deadlines."
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: key_highlights_Project_Alpha_is_80_complete_Beta

                    y: 90.80

                    height: 91
                    width: 209

                    color: "#4a5565"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 22.75
                    lineHeightMode: Text.FixedHeight
                    text: "Key highlights:\n- Project Alpha is 80% complete\n- Beta testing starts next week\n- Budget is within expected range"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: let_me_know_if_you_have_any_questions_

                    y: 204.20

                    height: 23
                    width: 244

                    color: "#4a5565"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 22.75
                    lineHeightMode: Text.FixedHeight
                    text: "Let me know if you have any questions."
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: best_Sarah

                    y: 249.60

                    height: 46
                    width: 36

                    color: "#4a5565"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 22.75
                    lineHeightMode: Text.FixedHeight
                    text: "Best,\nSarah"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
        Image {
            id: horizontalBorder_1

            y: 1129.60

            source: Qt.resolvedUrl("assets/horizontalBorder_3.png")

            Rectangle {
                id: buttonToReply

                x: 16
                y: 16.80

                height: 37.60
                width: 66

                color: "#155dfc"
                radius: 10

                Text {
                    id: reply

                    x: 16
                    y: 8.80

                    height: 20
                    width: 35

                    color: "#ffffff"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    text: "Reply"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Rectangle {
                id: buttonToForward

                x: 90
                y: 16.80

                height: 37.60
                width: 84.60

                border.color: "#e5e7eb"
                border.width: 1
                color: "transparent"
                radius: 10

                Text {
                    id: forward

                    x: 16.80
                    y: 8.80

                    height: 20
                    width: 52

                    color: "#4a5565"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    text: "Forward"
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Rectangle {
                id: _margin

                x: 182.60
                y: 16.80

                height: 37.60
                width: 1146.40

                color: "transparent"

                Rectangle {
                    id: container_8

                    x: 1074.40

                    height: 37.60
                    width: 72

                    color: "transparent"

                    Rectangle {
                        id: buttonToArchiveEmail

                        height: 37.60
                        width: 34

                        color: "transparent"
                        radius: 10

                        Rectangle {
                            id: sVG_2

                            x: 8
                            y: 9.80

                            height: 18
                            width: 18

                            clip: true
                            color: "transparent"

                            Shape {
                                id: _vector_4

                                x: 1.50
                                y: 2.25

                                height: 3.75
                                width: 15

                                ShapePath {
                                    id: _vector_4_ShapePath0

                                    fillColor: "#00000000"
                                    strokeColor: "#4a5565"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_4_ShapePath0_PathSvg0

                                        path: "M 0.75 0 L 14.25 0 C 14.664213582873344 0 15 0.3357864171266556 15 0.75 L 15 3 C 15 3.4142135828733444 14.664213582873344 3.75 14.25 3.75 L 0.75 3.75 C 0.3357864171266556 3.75 0 3.4142135828733444 0 3 L 0 0.75 C 0 0.3357864171266556 0.3357864171266556 0 0.75 0 Z"
                                    }
                                }
                            }
                            Shape {
                                id: _vector_5

                                x: 3
                                y: 6

                                height: 9.75
                                width: 12

                                ShapePath {
                                    id: _vector_5_ShapePath0

                                    fillColor: "#00000000"
                                    strokeColor: "#4a5565"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_5_ShapePath0_PathSvg0

                                        path: "M 0 0 L 0 8.25 C 6.661338147750939e-16 8.647824749350548 0.15803519636392593 9.029355324804783 0.4393397569656372 9.310659885406494 C 0.7206443175673485 9.591964446008205 1.1021752506494522 9.75 1.5 9.75 L 10.5 9.75 C 10.897824749350548 9.75 11.279355324804783 9.591964446008205 11.560659885406494 9.310659885406494 C 11.841964446008205 9.029355324804783 12 8.647824749350548 12 8.25 L 12 0"
                                    }
                                }
                            }
                            Shape {
                                id: _vector_6

                                x: 7.50
                                y: 9

                                height: 0
                                width: 3

                                ShapePath {
                                    id: _vector_6_ShapePath0

                                    fillColor: "#00000000"
                                    strokeColor: "#4a5565"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_6_ShapePath0_PathSvg0

                                        path: "M 0 0 L 3 0"
                                    }
                                }
                            }
                        }
                    }
                    Rectangle {
                        id: buttonToDeleteEmail

                        x: 38

                        height: 37.60
                        width: 34

                        color: "transparent"
                        radius: 10

                        Rectangle {
                            id: sVG_3

                            x: 8
                            y: 9.80

                            height: 18
                            width: 18

                            clip: true
                            color: "transparent"

                            Shape {
                                id: _vector_7

                                x: 2.25
                                y: 4.50

                                height: 0
                                width: 13.50

                                ShapePath {
                                    id: _vector_7_ShapePath0

                                    fillColor: "#00000000"
                                    strokeColor: "#4a5565"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_7_ShapePath0_PathSvg0

                                        path: "M 0 0 L 13.5 0"
                                    }
                                }
                            }
                            Shape {
                                id: _vector_8

                                x: 3.75
                                y: 4.50

                                height: 12
                                width: 10.50

                                ShapePath {
                                    id: _vector_8_ShapePath0

                                    fillColor: "#00000000"
                                    strokeColor: "#4a5565"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_8_ShapePath0_PathSvg0

                                        path: "M 10.5 0 L 10.5 10.5 C 10.5 11.25 9.75 12 9 12 L 1.5 12 C 0.75 12 0 11.25 0 10.5 L 0 0"
                                    }
                                }
                            }
                            Shape {
                                id: _vector_9

                                x: 6
                                y: 1.50

                                height: 3
                                width: 6

                                ShapePath {
                                    id: _vector_9_ShapePath0

                                    fillColor: "#00000000"
                                    strokeColor: "#4a5565"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_9_ShapePath0_PathSvg0

                                        path: "M 0 3 L 0 1.5 C 0 0.75 0.75 0 1.5 0 L 4.5 0 C 5.25 0 6 0.75 6 1.5 L 6 3"
                                    }
                                }
                            }
                            Shape {
                                id: _vector_10

                                x: 7.50
                                y: 8.25

                                height: 4.50
                                width: 0

                                ShapePath {
                                    id: _vector_10_ShapePath0

                                    fillColor: "#00000000"
                                    strokeColor: "#4a5565"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_10_ShapePath0_PathSvg0

                                        path: "M 0 0 L 0 4.5"
                                    }
                                }
                            }
                            Shape {
                                id: _vector_11

                                x: 10.50
                                y: 8.25

                                height: 4.50
                                width: 0

                                ShapePath {
                                    id: _vector_11_ShapePath0

                                    fillColor: "#00000000"
                                    strokeColor: "#4a5565"
                                    strokeWidth: 1.50

                                    PathSvg {
                                        id: _vector_11_ShapePath0_PathSvg0

                                        path: "M 0 0 L 0 4.5"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}