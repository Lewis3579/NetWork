import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Dialogs

Window {
    id: _window
    width: 1000
    height: 800
    visible: true
    x : Screen.width/2 - width/2
    y : Screen.height/2 - height/2
    title: qsTr("Hello World")


    Image {
        id: image
        x: 495
        y: 371
        width: 100
        height: 100
        source: "../Image/Test.jpg"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: _text
        x: 453
        y: 206
        text: qsTr("Text")
        font.pixelSize: 12
    }

    Loader{
        id: pageLoader
    }

    Button {
        id: button
        x: 216
        y: 371
        text: qsTr("ChooseFile")
        onClicked: {
            fileDialog.open()
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        currentFolder: "D:\\"

        onAccepted: {
            console.log("You choose: " + fileDialog.selectedFile)
        }

        onRejected: {
            console.log("Canceled")
        }
    }

}
