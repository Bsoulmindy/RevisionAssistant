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

    backButtonItem.onClicked: stackView.pop()
    selectDictButtonItem.onClicked: fileDialogItem.open()
    selectSeparatorHelpItem.onClicked: separatorHelpDialogItem.open()
    addDictButtonItem.onClicked: {
        if (fileDialogItem.selectedFile === "" || searchTextFieldItem.text === "") {
            invalidFileDialogItem.open()
        } else {
            stackView.push("DictProcessingPage.qml", {"filePath": fileDialogItem.selectedFile, "fileSeparator": searchTextFieldItem.text})
        }
    }
    separatorHelpTextItem.onLineLaidOut: (line)=> {
        if (line.y + separatorHelpTextItem.topPadding <= separator_example_item.y + separator_example_item.height) {
            line.width = separator_example_item.x - separatorHelpTextItem.anchors.leftMargin - separatorHelpTextItem.anchors.rightMargin;
        }
    }
    fileDialogItem.currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
    fileDialogItem.onAccepted: {
        fileSelectionStatusItem.state = "completed"
    }
}
