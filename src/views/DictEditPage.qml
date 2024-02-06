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
    listViewItem.model: dictController.getAllRecords()
    searchTextFieldItem.onAccepted: {
        listViewItem.searchText = searchTextFieldItem.text
    }
    searchButtonItem.onClicked: {
        listViewItem.searchText = searchTextFieldItem.text
    }
    StackView.onRemoved: {
        dictController.init()
    }
}
