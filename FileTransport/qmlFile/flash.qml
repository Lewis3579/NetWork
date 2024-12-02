import QtQuick

Item {
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        rotation: 0.069

        AnimatedImage {
            id: animatedImage
            x: 205
            y: 144
            width: 100
            height: 100
            source: "qrc:/qtquickplugin/images/template_image.png"
        }
    }

}
