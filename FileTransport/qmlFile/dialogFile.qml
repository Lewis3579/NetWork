import QtQuick
import QtQuick.Dialogs


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
    Component.onCompleted: visible = true
}
