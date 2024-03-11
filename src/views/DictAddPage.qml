import QtQuick 2.15
import QtQuick.Controls 2.15

import RevisionAssistant
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import QtQuick.Dialogs
import QtCore
import CustomComponents

DictAddPageUi {
    width: root.width
    height: root.height

    FileSelectorController {
        id: fileSelectorController

        onFile_nameChanged: (fileName) => {
            fileSelectionStatusItem.state = fileName !== "" ? "completed" : ""
        }
    }

    backButtonItem.onClicked: stackView.pop()
    selectDictButtonItem.onClicked: fileSelectorController.openFile("Text (*.txt)")
    selectSeparatorHelpItem.onClicked: separatorHelpDialogItem.open()
    addDictButtonItem.onClicked: {
        if (fileSelectorController.file_name === "" || searchTextFieldItem.text === "") {
            invalidFileDialogItem.open()
        } else {
            stackView.push("DictProcessingPage.qml", {"fileBytes": fileSelectorController.getFileBytes(), "fileSeparator": searchTextFieldItem.text})
        }
    }
    separatorHelpTextItem.onLineLaidOut: (line)=> {
        if (line.y + separatorHelpTextItem.topPadding <= separator_example_item.y + separator_example_item.height) {
            line.width = separator_example_item.x - separatorHelpTextItem.anchors.leftMargin - separatorHelpTextItem.anchors.rightMargin;
        }
    }
}
