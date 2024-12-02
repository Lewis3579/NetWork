import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Image {
        id: image
        x: 225
        y: 201
        width: 100
        height: 100
        source: "qrc:/Image/Test.jpg"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: _text
        text: qsTr("False")
        anchors.fill: parent
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        font.pixelSize: 12
    }
}
