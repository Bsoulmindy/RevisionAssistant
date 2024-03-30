import QtQuick 6.5
import QtQuick.Controls
import RevisionAssistant
import CustomComponents

DictsPageUi {
    width: root.width
    height: root.height

    backButtonItem.onClicked: stackView.pop()
    optionsListItem.model: dictFilesController.files
    useButtonItem.onClicked: dictFilesController.use_database_file(dictFilesController.selected_file)
    removeButtonItem.onClicked: removeDialog.open()
    newButtonItem.onClicked: newDictDialog.open()
    defaultButtonItem.enabled: dictFilesController.default_file !== dictFilesController.selected_file
    defaultButtonItem.onClicked: dictFilesController.mark_default_database_file(dictFilesController.selected_file)
    importButtonItem.onClicked: dictFilesController.import_database_file()
    exportButtonItem.onClicked: dictFilesController.export_database_file()
    renameButtonItem.onClicked: renameDictDialog.open()


    DictFilesController {
        id: dictFilesController
        dict_controller: dictController

        Component.onCompleted: {
            init()
        }

        onError: (message) => {
            if(message === "") {
                message = "Unknown error occured! Please restart the app!";
            }
            errorDialog.dialogText.text = message;
            errorDialog.open();
        }
    }

    Dialog {
        id: newDictDialog
        title: "New Dict"
        anchors.centerIn: parent
        width: 350
        height: 200
        contentItem: Item {
            TextField {
                id: newDictFileName
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                placeholderText: "Dict Name"
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        Overlay.modal: Rectangle {
            color: "#000000"
            opacity: 0.5
        }

        onAccepted: {
            dictFilesController.add_new_empty_file(newDictFileName.text, 0)
        }
    }

    Dialog {
        id: renameDictDialog
        title: "New Dict Name"
        anchors.centerIn: parent
        width: 350
        height: 200
        contentItem: Item {
            TextField {
                id: renameDictFileName
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                placeholderText: "Dict Name"
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        Overlay.modal: Rectangle {
            color: "#000000"
            opacity: 0.5
        }

        onAccepted: {
            dictFilesController.rename_database_file(renameDictFileName.text)
        }
    }

    Dialog {
        id: removeDialog
        title: "Warning"
        anchors.centerIn: parent
        width: 350
        height: 200
        contentItem: Item {
            PrimaryText {
                height: contentHeight
                text: "This action will delete the selected file PERMANENTLY, are you sure you want to proceed?"
                anchors.fill: parent
                horizontalAlignment: Text.AlignLeft
                anchors.rightMargin: 20
                anchors.leftMargin: 20
                anchors.bottomMargin: 20
                anchors.topMargin: 20
                font.pixelSize: 13
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        Overlay.modal: Rectangle {
            color: "#000000"
            opacity: 0.5
        }

        onAccepted: {
            dictFilesController.remove_database_file(dictFilesController.selected_file)
        }
    }
}
