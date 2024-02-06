import QtQuick 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts

Dialog {
    property alias dialogText: dialogTextContent

    title: "Info"
    anchors.centerIn: parent
    width: 350
    height: dialogTextContent.contentHeight + 150
    contentItem: Item {
        PrimaryText {
            id: dialogTextContent
            height: contentHeight
            text: ""
            anchors.fill: parent
            horizontalAlignment: Text.AlignLeft
            anchors.rightMargin: 20
            anchors.leftMargin: 20
            anchors.bottomMargin: 20
            anchors.topMargin: 20
            font.pixelSize: 13
        }
    }

    standardButtons: Dialog.Ok

    Overlay.modal: Rectangle {
        color: "#000000"
        opacity: 0.5
    }
}
