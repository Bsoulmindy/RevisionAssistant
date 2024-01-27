

import QtQuick 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts
import RevisionAssistant
import CustomComponents

Window {
    id: root
    width: 400
    height: 800
    visible: true
    title: qsTr("Revision Assistant")
    color: Material.backgroundColor

    DictController {
        id: dictController
        onError: (message) => {
            if(message === "") {
                message = "Failed to check the response due to some unknown error. Please restart the app!";
            }
            errorDalogText.text = message;
            errorDialog.open();
        }
    }

    FontLoader {
        id: globalFont
        source: "qrc:/fonts/stratum2demium.ttf"
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainPage {}

        pushEnter: Transition {
            PropertyAnimation {
                property: "scale"
                from: 0.8
                to: 1.0
                duration: 200
            }
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 200
            }
        }

        pushExit: Transition {
            PropertyAnimation {
                property: "scale"
                from: 1.0
                to: 1.2
                duration: 200
            }
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 200
            }
        }

        popEnter: Transition {
            PropertyAnimation {
                property: "scale"
                from: 1.2
                to: 1.0
                duration: 200
            }
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 200
            }
        }

        popExit: Transition {
            PropertyAnimation {
                property: "scale"
                from: 1.0
                to: 0.8
                duration: 200
            }
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 200
            }
        }
    }

    Dialog {
        id: errorDialog
        title: "Error"
        anchors.centerIn: parent
        width: 350
        contentItem: Item {
            PrimaryText {
                id: errorDalogText
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
}
