import QtQuick
import QtQuick.Shapes

Rectangle {
    height: 92
    color: "#ffffff"

    property bool starred: false
    property string theme: "ThemeHolder"
    property string name: "NameHolder"
    property string preview: "TextHolderTextHolderTextHolderTextHolde...."
    property string time: "10:30 AM"

    //Bottom border
    Rectangle
    {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 1
        color: "#e5e7eb"
    }

    Rectangle {
        id: container_1

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 16
        }

        height: 60
        color: "transparent"

        // TEMP ICON
        Rectangle {
            id: button_margin_2
            height: 17
            width: 15
            anchors.verticalCenter: parent.verticalCenter
            color: "transparent"

            Rectangle {
                id: buttonToFavouriteHolder_1
                y: 2
                height: 15
                width: 15
                clip: true
                color: "transparent"

                Shape {
                    id: _vector_2
                    x: 1.25; y: 1.25
                    height: 11.92; width: 12.50

                    ShapePath {
                        fillColor: "#00000000"
                        fillRule: ShapePath.WindingFill
                        strokeColor: "#6a7282"
                        strokeWidth: 1.25
                        PathSvg {
                            path: "M 5.954249387670271 0.18430858481834558 C 5.981639027602105 0.12897159180617918 6.023950033486466 0.08239125435289907 6.076407316668323 0.04982426662924932 C 6.12886459985018 0.017257278905599575 6.189380184051501 2.775557506047615e-16 6.251124620426914 0 C 6.312869056802327 0 6.373384641003649 0.017257278905599575 6.425841924185506 0.04982426662924932 C 6.478299207367363 0.08239125435289907 6.520610213251723 0.12897159180617918 6.5479998531835575 0.18430858481834558 L 7.991749491862689 3.10868346433767 C 8.08686033300685 3.3011630262274245 8.22725751578896 3.4676886680782006 8.400890070912682 3.593966889022302 C 8.574522626036405 3.7202451099664033 8.77620393436359 3.8025040396822414 8.988624043697087 3.833683354469163 L 12.217374807047385 4.306183669267722 C 12.278553097097099 4.315047585827458 12.33603044634765 4.340853352820459 12.383305790287208 4.380682917235939 C 12.430581134226767 4.420512481651419 12.46576784385748 4.472776203361082 12.484886408161897 4.53156253572386 C 12.504004972466314 4.590348868086638 12.50629319357145 4.653312442883336 12.491490602677057 4.713331068655907 C 12.476688011782665 4.773349694428478 12.445385861342272 4.828028335525817 12.40112519454941 4.871183238350191 L 10.066125200935016 7.144933796070806 C 9.912134890461124 7.294996933387117 9.79691919347008 7.480234614900541 9.730396681222535 7.68470152743163 C 9.66387416897499 7.889168439962719 9.648037154624241 8.106739079483006 9.684249574024639 8.318683934718692 L 10.235500140484277 11.531183488994198 C 10.24629937384715 11.592332460952973 10.239693725304647 11.655285595623818 10.216436191262899 11.712860826821563 C 10.19317865722115 11.770436058019309 10.154205814060385 11.8203143762814 10.10396461290256 11.856806041075012 C 10.053723411744736 11.893297705868624 9.994236198567304 11.91493287221272 9.932290722248156 11.919243097321296 C 9.870345245929009 11.923553322429871 9.808435453233626 11.910365192878846 9.753624610848506 11.881183148207674 L 6.86737464682091 10.363683493272447 C 6.677197798400475 10.263824835018866 6.46561217270437 10.2116523130705 6.250812292094248 10.211652313070498 C 6.036012411484126 10.211652313070498 5.824426189741584 10.263824835018866 5.63424934132115 10.363683493272447 L 2.748624630005322 11.881183148207674 C 2.693831710922809 11.910190143819529 2.631998035219998 11.923237935284975 2.57015670404577 11.918842554116416 C 2.508315372871542 11.914447172947858 2.4489486309234527 11.892784747374625 2.398809148815288 11.856319667183374 C 2.3486696667071234 11.819854586992124 2.3097702303902876 11.770050923546119 2.286534861825393 11.71257234034662 C 2.263299493260498 11.655093757147121 2.256660950405288 11.592248097099239 2.2673746511045385 11.531183488994198 L 2.81799966682919 8.319308591383443 C 2.8543722643961416 8.107265984532281 2.8386145614743343 7.889557248503394 2.772085911811139 7.684962595770525 C 2.705557262147944 7.480367943037656 2.590254989412015 7.295030004651081 2.4361246359652493 7.144933796070806 L 0.10112481930214008 4.871808491061379 C 0.056495958793039446 4.828703791179385 0.024870561427154388 4.773930928179057 0.009851332100353107 4.713729823721478 C -0.005167897226448172 4.6535287192639 -0.0029771064305997956 4.590319466233545 0.01617404368592932 4.531302659477837 C 0.03532519380245844 4.472285852722129 0.07066671136103211 4.4198343903550175 0.11817279046405042 4.379922958030253 C 0.16567886956706873 4.3400115257054885 0.22343946194683398 4.31424494760149 0.2848747504561122 4.305558416556534 L 3.512999646421754 3.833683354469163 C 3.7256600928542722 3.802745415205689 3.927617743210528 3.720594346611673 4.101489704110715 3.59430037700315 C 4.275361665010902 3.4680064073946273 4.415939431236786 3.301352718005628 4.511124405656472 3.10868346433767 L 5.954249387670271 0.18430858481834558 Z"
                        }
                    }
                }
            }
        }

        // Content of item
        Rectangle {
            id: container_2

            anchors.left: button_margin_2.right
            anchors.leftMargin: 8
            anchors.right: parent.right
            height: 60
            color: "transparent"

            Rectangle {
                id: container_3
                anchors.left: parent.left
                anchors.right: parent.right
                height: 20
                color: "transparent"

                Rectangle {
                    id: nameHolder
                    anchors.left: parent.left
                    anchors.right: timeHolder.left
                    height: 20
                    clip: true
                    color: "transparent"

                    Text {
                        id: nameHolder_1
                        height: 20
                        width: parent.width
                        color: "#101828"
                        font.family: "Segoe UI"
                        font.pixelSize: 14
                        font.weight: Font.Black
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                        text: name
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }

                Rectangle {
                    id: timeHolder
                    anchors.right: parent.right
                    y: 2
                    height: 16
                    width: 59
                    color: "transparent"

                    Text {
                        id: aM
                        x: 8
                        height: 16
                        width: 52
                        color: "#6a7282"
                        font.family: "Segoe UI"
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        lineHeight: 16
                        lineHeightMode: Text.FixedHeight
                        text: time
                        textFormat: Text.PlainText
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            // Theme of item
            Rectangle {
                id: themeHolder
                anchors.left: parent.left
                anchors.right: parent.right
                y: 22
                height: 20
                clip: true
                color: "transparent"

                Text {
                    id: themeHolder_1
                    height: 20
                    width: parent.width
                    color: "#101828"
                    font.family: "Segoe UI"
                    font.pixelSize: 14
                    font.weight: Font.Black
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    text: theme
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                }
            }

            // Text preview
            Rectangle {
                id: textHolder
                anchors.left: parent.left
                anchors.right: parent.right
                y: 44
                height: 16
                clip: true
                color: "transparent"

                Text {
                    id: textHolderText
                    height: 16
                    width: parent.width
                    color: "#6a7282"
                    font.family: "Segoe UI"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    text: preview
                    textFormat: Text.PlainText
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                }
            }
        }
    }
}