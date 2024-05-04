import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import RevisionAssistant
import CustomComponents

DictEditPageUi {
    id: dictEditPage
    width: root.width
    height: root.height

    backButtonItem.onClicked: stackView.pop()
    listViewItem.model: dictController.getAllRecords("")
    searchTextFieldItem.onAccepted: {
        listViewItem.model = dictController.getAllRecords(searchTextFieldItem.text)
    }
    searchButtonItem.onClicked: {
        listViewItem.model = dictController.getAllRecords(searchTextFieldItem.text)
    }
    StackView.onRemoved: {
        dictController.init()
    }
    addButtonItem.onClicked: {
        addDialog.open()
    }

    Dialog {
        id: addDialog
        title: "Add"
        anchors.centerIn: parent
        width: 350
        height: 300
        contentItem: Item {
            TextField {
                id: qAddDialog
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 10
                placeholderText: "Question"
            }

            TextField {
                id: rAddDialog
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: qAddDialog.bottom
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 20
                placeholderText: "Response"
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        Overlay.modal: Rectangle {
            color: "#000000"
            opacity: 0.5
        }

        onAccepted: {
            if(dictController.insertNewEntry(qAddDialog.text, rAddDialog.text)) {
                listViewItem.model = dictController.getAllRecords(searchTextFieldItem.text)
                listViewItem.positionViewAtEnd()
            }
        }
    }
}
