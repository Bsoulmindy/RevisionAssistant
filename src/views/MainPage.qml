

import QtQuick 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts
import RevisionAssistant
import "../components"

Page {
    MainPageUi {
        width: root.width
        height: root.height

        dictMenuButtonItem.onClicked: dictMenu.open()
        menuItemOpenItem.onTriggered: stackView.push(DictAddPage)
        menuItemEditItem.onTriggered: stackView.push("DictEditPage.qml")
        menuItemResetItem.onTriggered: resetDialog.open()
        menuItemInfoItem.onTriggered: aboutDialog.open()
        qTorButtonItem.onClicked: stackView.push("QrPage.qml", {"isQtoR": true})
        rToqButtonItem.onClicked: stackView.push("QrPage.qml", {"isQtoR": false})
        resetDialogItem.onAccepted: dictController.resetDict()
        githubLinkItem.onClicked: Qt.openUrlExternally("https://github.com/Bsoulmindy")
    }
}
