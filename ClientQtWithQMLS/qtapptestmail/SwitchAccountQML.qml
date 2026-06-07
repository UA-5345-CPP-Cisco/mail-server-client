import QtQuick

Rectangle {
    id: switchAccountQML

    height: 199
    width: 287

    clip: true
    color: "#ffffff"
    radius: 13

    Image {
        id: background_Border_Shadow

        x: -38
        y: -13

        clip: true
        source: Qt.resolvedUrl("assets/background_Border_Shadow.png")
    }
}