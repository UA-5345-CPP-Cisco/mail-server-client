import QtQuick
import QtQuick.Shapes
import QtQuick.Layouts

Rectangle {
    id: settingsContainer

    implicitHeight: 313
    implicitWidth: 382

    clip: true
    color: "#ffffff"
    radius: 14

    // ==========================================
    // HEADER
    // ==========================================
    Rectangle {
        id: headerContainer

        x: 16
        y: 16
        height: 24
        width: 65
        color: "transparent"

        Text {
            id: settingsTitleText

            height: 24
            width: 66
            color: "#101828"
            font.family: "Segoe UI"
            font.pixelSize: 16
            font.weight: Font.Black
            horizontalAlignment: Text.AlignLeft
            lineHeight: 24
            lineHeightMode: Text.FixedHeight
            text: "Settings"
            textFormat: Text.PlainText
            verticalAlignment: Text.AlignVCenter
        }

        Rectangle {
            id: closeButtonWrapper

            x: 348.40
            y: 19
            height: 18
            width: 18
            color: "transparent"

            Rectangle {
                id: closeIconContainer

                height: 18
                width: 18
                clip: true
                color: "transparent"

                MouseArea {
                    id: closeSettingsMouseArea
                    anchors.fill: parent

                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        closeSettingsWindow()
                    }
                }

                scale: closeSettingsMouseArea.containsMouse ? 1.5 : 1.0

                Behavior on scale {
                    NumberAnimation {
                        duration: 150
                        easing.type: Easing.InOutQuad
                    }
                }

                Image {
                    source: "qrc:/pngs/assets/ic_close_window.svg"
                    width: 14
                    height: 14
                    sourceSize.width: 120
                    sourceSize.height: 120
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }
        }
    }

    // ==========================================
    // FOOTER (VERSION)
    // ==========================================
    Rectangle {
        id: versionWrapper

        x: 11
        y: 284
        height: 16
        width: 350.40
        color: "transparent"

        Text {
            id: versionText

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
            text: "Mail App v1.0"
            textFormat: Text.PlainText
            verticalAlignment: Text.AlignVCenter
        }
    }

    // ==========================================
    // PREFERENCES LIST
    // ==========================================
    Rectangle {
        id: preferencesContainer

        y: 64
        height: 220
        width: 382.40
        color: "transparent"

        // 1. Theme Settings Row
        Rectangle {
            id: themeSettingsContainer

            x: 16
            y: 16
            height: 60
            width: 350.40
            color: "transparent"
            radius: 10

            Rectangle {
                id: themeIconWrapper

                x: 16
                y: 12
                height: 36
                width: 106
                color: "transparent"

                Rectangle {
                    id: themeIconSvg

                    y: 9
                    height: 18
                    width: 18
                    clip: true
                    color: "transparent"

                    Shape {
                        id: themeVector1
                        x: 6; y: 6; height: 6; width: 6
                        ShapePath {
                            fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                                path: "M 6 3 C 6 4.656854331493378 4.656854331493378 6 3 6 C 1.3431456685066223 6 0 4.656854331493378 0 3 C 0 1.3431456685066223 1.3431456685066223 0 3 0 C 4.656854331493378 0 6 1.3431456685066223 6 3 Z"
                            }
                        }
                    }
                    Shape {
                        id: themeVector2
                        x: 9; y: 1.50; height: 1.50; width: 0
                        ShapePath {
                            fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                                path: "M 0 0 L 0 1.5"
                            }
                        }
                    }
                    Shape {
                        id: themeVector3
                        x: 9; y: 15; height: 1.50; width: 0
                        ShapePath {
                            fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                                path: "M 0 0 L 0 1.5"
                            }
                        }
                    }
                    Shape {
                        id: themeVector4
                        x: 3.70; y: 3.70; height: 1.06; width: 1.06
                        ShapePath {
                            fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                                path: "M 0 0 L 1.0575002431869507 1.0575002431869507"
                            }
                        }
                    }
                    Shape {
                        id: themeVector5
                        x: 13.24; y: 13.24; height: 1.06; width: 1.06
                        ShapePath {
                            fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                                path: "M 0 0 L 1.057499885559082 1.057499885559082"
                            }
                        }
                    }
                    Shape {
                        id: themeVector6
                        x: 1.50; y: 9; height: 0; width: 1.50
                        ShapePath {
                            fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                                path: "M 0 0 L 1.5 0"
                            }
                        }
                    }
                    Shape {
                        id: themeVector7
                        x: 15; y: 9; height: 0; width: 1.50
                        ShapePath {
                            fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                                path: "M 0 0 L 1.5 0"
                            }
                        }
                    }
                    Shape {
                        id: themeVector8
                        x: 3.70; y: 13.24; height: 1.06; width: 1.06
                        ShapePath {
                            fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                                path: "M 1.0575002431869507 0 L 0 1.057499885559082"
                            }
                        }
                    }
                    Shape {
                        id: themeVector9
                        x: 13.24; y: 3.70; height: 1.06; width: 1.06
                        ShapePath {
                            fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                                path: "M 1.057499885559082 0 L 0 1.0575002431869507"
                            }
                        }
                    }
                }

                Rectangle {
                    id: themeTextWrapper

                    x: 30
                    height: 36
                    width: 76
                    color: "transparent"

                    Rectangle {
                        id: themeTitleWrapper

                        height: 20
                        width: 76
                        color: "transparent"

                        Text {
                            id: themeTitleText

                            height: 20
                            width: 70
                            color: "#101828"
                            font.family: "Segoe UI"
                            font.pixelSize: 14
                            font.weight: Font.Normal
                            horizontalAlignment: Text.AlignLeft
                            lineHeight: 20
                            lineHeightMode: Text.FixedHeight
                            text: "Theme"
                            textFormat: Text.PlainText
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Rectangle {
                        id: themeSubtitleWrapper

                        y: 20
                        height: 16
                        width: 76
                        color: "transparent"

                        Text {
                            id: themeSubtitleText

                            height: 16
                            width: 90
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

            // ==========================================
            // ANIMATED SLIDER (Replaces the old toggle)
            // ==========================================
            Rectangle {
                id: themeAnimatedSlider

                x: 200 // Positioned nicely on the right
                y: 14
                height: 32
                width: 140

                color: "#f1f5f9"
                radius: 8

                property int currentIndex: 0
                property var options: ["Light", "Dark"]

                Rectangle {
                    id: sliderPod

                    width: (parent.width - 8) / themeAnimatedSlider.options.length
                    height: parent.height - 8
                    x: 4 + (themeAnimatedSlider.currentIndex * width)
                    y: 4

                    color: "#ffffff"
                    radius: 6
                    border.color: "#e2e8f0"
                    border.width: 1

                    Behavior on x {
                        NumberAnimation {
                            duration: 200
                            easing.type: Easing.OutCubic
                        }
                    }
                }

                Row {
                    anchors.fill: parent

                    Repeater {
                        model: themeAnimatedSlider.options

                        delegate: Item {
                            required property int index
                            required property string modelData

                            width: themeAnimatedSlider.width / themeAnimatedSlider.options.length
                            height: themeAnimatedSlider.height

                            Text {
                                anchors.centerIn: parent
                                text: modelData
                                font.family: "Segoe UI"
                                font.pixelSize: 12
                                font.weight: themeAnimatedSlider.currentIndex === index ? Font.DemiBold : Font.Normal
                                color: themeAnimatedSlider.currentIndex === index ? "#0f172a" : "#64748b"

                                Behavior on color {
                                    ColorAnimation {
                                        duration: 150
                                    }
                                }
                            }

                            TapHandler {
                                onTapped: {
                                    themeAnimatedSlider.currentIndex = index;
                                    themeSubtitleText.text = "Currently " + modelData.toLowerCase();
                                }
                            }
                        }
                    }
                }
            }
        }

        // 2. Account Settings Row
        Rectangle {
            id: accountSettingsButton

            x: 16
            y: 80
            height: 60
            width: 350.40
            radius: 10
            color: clickAreaAccountSettings.hovered ? "#fff0f0" : "transparent"

            HoverHandler {
                id: clickAreaAccountSettings
                cursorShape: Qt.PointingHandCursor
            }

            TapHandler {
                onTapped: {
                    // Add your routing logic here
                }
            }

            Rectangle {
                id: accountIconContainer

                x: 16
                y: 21
                height: 18
                width: 18
                clip: true
                color: "transparent"

                Shape {
                    id: accountVector1
                    x: 3.75; y: 11.25; height: 4.50; width: 10.50
                    ShapePath {
                        fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                            path: "M 10.5 4.5 L 10.5 3 C 10.5 2.2043505012989044 10.18392889201641 1.4412893503904343 9.621319770812988 0.8786802291870117 C 9.058710649609566 0.3160711079835892 8.295649498701096 2.6645352591003757e-15 7.5 0 L 3 0 C 2.2043505012989044 1.3322676295501878e-15 1.441288635134697 0.3160711079835892 0.8786795139312744 0.8786802291870117 C 0.31607039272785187 1.4412893503904343 6.661338147750939e-16 2.2043505012989044 0 3 L 0 4.5"
                        }
                    }
                }
                Shape {
                    id: accountVector2
                    x: 6; y: 2.25; height: 6; width: 6
                    ShapePath {
                        fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                            path: "M 6 3 C 6 4.656854331493378 4.656854331493378 6 3 6 C 1.3431456685066223 6 0 4.656854331493378 0 3 C 0 1.3431456685066223 1.3431456685066223 0 3 0 C 4.656854331493378 0 6 1.3431456685066223 6 3 Z"
                        }
                    }
                }
            }

            Rectangle {
                id: accountTextContainer

                x: 46
                y: 12
                height: 36
                width: 196
                color: "transparent"

                Rectangle {
                    id: accountTitleWrapper

                    height: 20
                    width: 196
                    color: "transparent"

                    Text {
                        id: accountTitleText

                        height: 20
                        width: 106
                        color: "#101828"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                        text: "Account Settings"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Rectangle {
                    id: accountSubtitleWrapper

                    y: 20
                    height: 16
                    width: 196
                    color: "transparent"

                    Text {
                        id: accountSubtitleText

                        height: 16
                        width: 197
                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 16
                        lineHeightMode: Text.FixedHeight
                        text: "Manage your profile and preferences"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        // 3. General Settings Row
        Rectangle {
            id: generalSettingsButton

            x: 16
            y: 144
            height: 60
            width: 350.40
            radius: 10
            color: clickAreaGeneralSettings.hovered ? "#fff0f0" : "transparent"

            HoverHandler {
                id: clickAreaGeneralSettings
                cursorShape: Qt.PointingHandCursor
            }

            TapHandler {
                onTapped: {
                    if (String(generalSettingsLoader.source) === "") {
                        generalSettingsLoader.active = true
                        generalSettingsLoader.source = "SettingsGeneralQML.qml"
                    } else {
                        generalSettingsLoader.source = ""
                    }
                }
            }

            Rectangle {
                id: generalIconContainer

                x: 16
                y: 21
                height: 18
                width: 18
                clip: true
                color: "transparent"

                Shape {
                    id: generalVector1
                    x: 2.24; y: 1.50; height: 15; width: 13.53
                    ShapePath {
                        fillColor: "#00000000"; fillRule: ShapePath.WindingFill; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                            path: "M 6.928185224533081 0 L 6.598184823989868 0 C 6.20036007463932 3.3306690738754696e-16 5.818829499185085 0.15803519636392593 5.537524938583374 0.4393397569656372 C 5.256220377981663 0.7206443175673485 5.098184823989868 1.1021752506494522 5.098184823989868 1.5 L 5.098184823989868 1.6349998712539673 C 5.097915017489868 1.8980440199375153 5.028477922081947 2.1563924700021744 4.896838903427124 2.384127974510193 C 4.765199884772301 2.6118634790182114 4.575987942516804 2.8009778782725334 4.348184823989868 2.932499885559082 L 4.0256853103637695 3.119999885559082 C 3.7976565062999725 3.251652382314205 3.538989946246147 3.320962071418762 3.275684952735901 3.320962071418762 C 3.0123799592256546 3.320962071418762 2.753713756799698 3.251652382314205 2.525684952735901 3.119999885559082 L 2.413184881210327 3.059999942779541 C 2.068983681499958 2.861447051167488 1.6600627899169922 2.80758286267519 1.276186466217041 2.910231113433838 C 0.8923101425170898 3.012879364192486 0.5648485869169235 3.2636517360806465 0.36568504571914673 3.6074999570846558 L 0.2006850242614746 3.8925000429153442 C 0.002132132649421692 4.236701242625713 -0.05173223465681076 4.645622491836548 0.05091601610183716 5.029498815536499 C 0.15356426686048508 5.41337513923645 0.4043368175625801 5.740836516022682 0.7481850385665894 5.940000057220459 L 0.8606851100921631 6.015000343322754 C 1.0873919650912285 6.145883247256279 1.2758994363248348 6.333816535770893 1.407473087310791 6.5601232051849365 C 1.5390467382967472 6.78642987459898 1.6091050548129715 7.04322923719883 1.610685110092163 7.304999828338623 L 1.610685110092163 7.6875 C 1.6117344612430315 7.951815001666546 1.542922753840685 8.211714416742325 1.4112178087234497 8.44088101387024 C 1.2795128636062145 8.670047610998154 1.0895916223526 8.860343366861343 0.8606851100921631 8.992499828338623 L 0.7481850385665894 9.059999942779541 C 0.4043368175625801 9.259163483977318 0.15356426686048508 9.58662486076355 0.05091601610183716 9.970501184463501 C -0.05173223465681076 10.354377508163452 0.002132132649421692 10.763298399746418 0.2006850242614746 11.107499599456787 L 0.36568504571914673 11.392500400543213 C 0.5648485869169235 11.736348621547222 0.8923101425170898 11.987120635807514 1.276186466217041 12.089768886566162 C 1.6600627899169922 12.19241713732481 2.068983681499958 12.138552948832512 2.413184881210327 11.940000057220459 L 2.525684952735901 11.880000114440918 C 2.753713756799698 11.748347617685795 3.0123799592256546 11.67903757095337 3.275684952735901 11.67903757095337 C 3.538989946246147 11.67903757095337 3.7976565062999725 11.748347617685795 4.0256853103637695 11.880000114440918 L 4.348184823989868 12.067500114440918 C 4.575987942516804 12.199022121727467 4.765199884772301 12.388136878609657 4.896838903427124 12.615872383117676 C 5.028477922081947 12.843607887625694 5.097915017489868 13.101955622434616 5.098184823989868 13.364999771118164 L 5.098184823989868 13.5 C 5.098184823989868 13.897824749350548 5.256220377981663 14.279355324804783 5.537524938583374 14.560659885406494 C 5.818829499185085 14.841964446008205 6.20036007463932 15 6.598184823989868 15 L 6.928185224533081 15 C 7.326009973883629 15 7.707540549337864 14.841964446008205 7.988845109939575 14.560659885406494 C 8.270149670541286 14.279355324804783 8.428185224533081 13.897824749350548 8.428185224533081 13.5 L 8.428185224533081 13.364999771118164 C 8.428455031033081 13.101955622434616 8.497892126441002 12.843607887625694 8.629531145095825 12.615872383117676 C 8.761170163750648 12.388136878609657 8.950382106006145 12.199022121727467 9.178185224533081 12.067500114440918 L 9.50068473815918 11.880000114440918 C 9.728713542222977 11.748347617685795 9.987379744648933 11.67903757095337 10.25068473815918 11.67903757095337 C 10.513989731669426 11.67903757095337 10.772655934095383 11.748347617685795 11.00068473815918 11.880000114440918 L 11.113184452056885 11.940000057220459 C 11.457385651767254 12.138552948832512 11.866307258605957 12.19241713732481 12.250183582305908 12.089768886566162 C 12.63405990600586 11.987120635807514 12.961521998047829 11.736348621547222 13.160685539245605 11.392500400543213 L 13.325685024261475 11.09999942779541 C 13.524237915873528 10.755798228085041 13.578102104365826 10.346877336502075 13.475453853607178 9.963001012802124 C 13.37280560284853 9.579124689102173 13.122033588588238 9.25166331231594 12.778185367584229 9.052499771118164 L 12.665685653686523 8.992499828338623 C 12.436779141426086 8.860343366861343 12.246856827288866 8.670047610998154 12.11515188217163 8.44088101387024 C 11.983446937054396 8.211714416742325 11.914636302535655 7.951815001666546 11.915685653686523 7.6875 L 11.915685653686523 7.3125 C 11.914636302535655 7.048184998333454 11.983446937054396 6.788285583257675 12.11515188217163 6.559118986129761 C 12.246856827288866 6.329952389001846 12.436779141426086 6.139656633138657 12.665685653686523 6.007500171661377 L 12.778185367584229 5.940000057220459 C 13.122033588588238 5.740836516022682 13.37280560284853 5.41337513923645 13.475453853607178 5.029498815536499 C 13.578102104365826 4.645622491836548 13.524237915873528 4.236701242625713 13.325685024261475 3.8925000429153442 L 13.160685539245605 3.6074999570846558 C 12.961521998047829 3.2636517360806465 12.63405990600586 3.012879364192486 12.250183582305908 2.910231113433838 C 11.866307258605957 2.80758286267519 11.457385651767254 2.861447051167488 11.113184452056885 3.059999942779541 L 11.00068473815918 3.119999885559082 C 10.772655934095383 3.251652382314205 10.513989731669426 3.320962071418762 10.25068473815918 3.320962071418762 C 9.987379744648933 3.320962071418762 9.728713542222977 3.251652382314205 9.50068473815918 3.119999885559082 L 9.178185224533081 2.932499885559082 C 8.950382106006145 2.8009778782725334 8.761170163750648 2.6118634790182114 8.629531145095825 2.384127974510193 C 8.497892126441002 2.1563924700021744 8.428455031033081 1.8980440199375153 8.428185224533081 1.6349998712539673 L 8.428185224533081 1.5 C 8.428185224533081 1.1021752506494522 8.270149670541286 0.7206443175673485 7.988845109939575 0.4393397569656372 C 7.707540549337864 0.15803519636392593 7.326009973883629 3.3306690738754696e-16 6.928185224533081 0 Z"
                        }
                    }
                }
                Shape {
                    id: generalVector2
                    x: 6.75; y: 6.75; height: 4.50; width: 4.50
                    ShapePath {
                        fillColor: "#00000000"; strokeColor: "#4a5565"; strokeWidth: 1.50; PathSvg {
                            path: "M 4.5 2.25 C 4.5 3.4926407039165497 3.4926407039165497 4.5 2.25 4.5 C 1.0073592960834503 4.5 0 3.4926407039165497 0 2.25 C 0 1.0073592960834503 1.0073592960834503 0 2.25 0 C 3.4926407039165497 0 4.5 1.0073592960834503 4.5 2.25 Z"
                        }
                    }
                }
            }

            Rectangle {
                id: generalTextContainer

                x: 46
                y: 12
                height: 36
                width: 180
                color: "transparent"

                Rectangle {
                    id: generalTitleWrapper

                    height: 20
                    width: 180
                    color: "transparent"

                    Text {
                        id: generalTitleText

                        height: 20
                        width: 49
                        color: "#101828"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                        text: "General"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Rectangle {
                    id: generalSubtitleWrapper

                    y: 20
                    height: 16
                    width: 180
                    color: "transparent"

                    Text {
                        id: generalSubtitleText

                        height: 16
                        width: 181
                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 16
                        lineHeightMode: Text.FixedHeight
                        text: "Notifications, language, and more"
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}