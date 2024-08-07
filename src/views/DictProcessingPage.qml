import QtQuick 2.15
import QtQuick.Controls 2.15

import RevisionAssistant
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import CustomComponents

DictProcessingPageUi {
    id: dictProcessingPage

    width: root.width
    height: root.height

    required property var fileBytes
    required property string fileSeparator
    required property string mode
    required property string entrySeparator

    FileController {
        id: fileController
        dict_controller: dictController
        onWarningOutput: (output) => {
            listViewItem.model.append({ "output": output });
        }
        onInitialized: {
            if (mode === "OneToOne") {
                fileController.constructDictFromBytes(fileBytes, fileSeparator)
            }
            if(mode === "ManyToMany") {
                fileController.constructMToMDictFromBytes(fileBytes, fileSeparator, entrySeparator)
            }
        }
        onError: (message) => {
            if(message === "") {
                message = "Unknown error occured! Please restart the app!";
            }
            errorDialog.dialogText.text = message;
            errorDialog.open();
        }
    }  

    closeButtonItem.onClicked: stackView.pop()
    statusTextItem.text: fileController.actualState
    backButtonItem.icon.source: "qrc:/icons/back.png"
    progressBarItem.value: fileController.actualState === "Finished" ? 1 : 0
    progressBarItem.indeterminate: fileController.actualState !== "Idle" && fileController.actualState !== "Finished"

    InfoDialog {
        id: errorDialog
        title: "Error"
        standardButtons: Dialog.Ok
    }
}
