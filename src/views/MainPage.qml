

import QtQuick 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts
import RevisionAssistant

Page {
    MainPageUi {
        width: root.width
        height: root.height

        dictMenuButtonItem.onClicked: dictMenuItem.open()
        menuItemOpenItem.onTriggered: stackView.push(dictAddPageComponent)
        menuItemEditItem.onTriggered: stackView.push(dictEditPageComponent)
        menuItemResetItem.onTriggered: resetDialogItem.open()
        menuItemInfoItem.onTriggered: aboutDialogItem.open()
        optionsButtonItem.onClicked: optionsMenuItem.open()
        qTorButtonItem.onClicked: stackView.push(qrPageComponent, {"isQtoR": true})
        rToqButtonItem.onClicked: stackView.push(qrPageComponent, {"isQtoR": false})
        resetDialogItem.onAccepted: dictController.resetDict()
        githubLinkItem.onClicked: Qt.openUrlExternally("https://github.com/Bsoulmindy")
    }

    Component {
        id: dictAddPageComponent
        DictAddPage {}
    }
    Component {
        id: dictEditPageComponent
        DictEditPage {}
    }
    Component {
        id: qrPageComponent
        QrPage {}
    }
}
