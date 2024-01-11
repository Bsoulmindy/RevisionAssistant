

import QtQuick 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts
import RevisionAssistant
import "components"

Page {
    id: mainPage
    width: root.width
    height: root.height
    objectName: "MainPage"

    ToolBar {
        id: mainPageToolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        spacing: 16
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 0

        RowLayout {
            x: 573
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 16

            ToolButton {
                id: dictMenuButton
                text: qsTr("")
                Layout.fillWidth: true
                icon.source: "qrc:/icons/dict_icon.png"
                icon.cache: true
                display: AbstractButton.IconOnly
                onClicked: dictMenu.open()

                Menu {
                    id: dictMenu
                    y: dictMenuButton.height

                    MenuItem {
                        height: 42
                        text: "Open"
                        icon.source: "qrc:/icons/open_file.png"
                        onTriggered: stackView.push("DictAddPage.qml")
                    }
                    MenuItem {
                        height: 42
                        text: "Edit"
                        icon.source: "qrc:/icons/edit.png"
                        onTriggered: stackView.push("DictEditPage.qml")
                    }
                    MenuItem {
                        text: "Reset"
                        height: 42
                        icon.source: "qrc:/icons/reset.png"
                        onTriggered: resetDialog.open()
                    }
                }
            }
            ToolButton {
                id: optionsButton
                width: 48
                text: qsTr("")
                Layout.fillWidth: true
                icon.source: "qrc:/icons/menu.png"
                display: AbstractButton.IconOnly
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    y: dictMenuButton.height

                    MenuItem {
                        text: "About"
                        height: 42
                        icon.source: "qrc:/icons/info.png"

                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    ColumnLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 16
        Button {
            id: qTorButton
            text: qsTr("Question To Response")
            Layout.fillWidth: true
            font.pointSize: 12
            Layout.preferredWidth: 205
            Layout.preferredHeight: 52
            Material.background: Material.primaryColor
            onClicked: stackView.push("QrPage.qml", {"isQtoR": true})
        }

        Button {
            id: rToqButton
            text: qsTr("Response To Question")
            Layout.fillWidth: true
            font.pointSize: 12
            Layout.preferredWidth: 205
            Layout.preferredHeight: 52
            Material.background: Material.primaryColor
            onClicked: stackView.push("QrPage.qml", {"isQtoR": false})
        }
    }

    Dialog {
        id: resetDialog
        title: "Reset Dialog"
        anchors.centerIn: parent
        width: 350
        height: 200
        contentItem: Item {
            PrimaryText {
                height: contentHeight
                text: "This action will reset all your progression, marking every question and response as NOT CHECKED. Do you want to proceed?"
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

        onAccepted: dictController.resetDict()


        Overlay.modal: Rectangle {
            color: "#000000"
            opacity: 0.5
        }
    }

    Dialog {
        id: aboutDialog
        title: "About"
        anchors.centerIn: parent
        width: 350
        height: 350

        contentItem: Item {
            ColumnLayout {
                anchors.fill: parent
                anchors.rightMargin: 20
                anchors.leftMargin: 20
                anchors.bottomMargin: 0
                anchors.topMargin: 0
                spacing: 10

                Image {
                    id: image
                    source: "qrc:/icons/revision_assistant_icon.png"
                    Layout.fillWidth: true
                    fillMode: Image.PreserveAspectFit
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 100
                }

                PrimaryText {
                    height: contentHeight
                    text: "Revision Assistant"
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                    font.pixelSize: 18
                    font.bold: true
                }

                PrimaryText {
                    height: contentHeight
                    text: "Version <b>" + appVersion + "</b>. Based on <b>Qt 6.6</b>"
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                    font.pixelSize: 15
                }

                PrimaryText {
                    height: contentHeight
                    text: "Created by Bsoulmindy"
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                    font.pixelSize: 15

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            Qt.openUrlExternally("https://github.com/Bsoulmindy")
                        }
                        cursorShape: Qt.PointingHandCursor
                    }
                }
            }
        }


        standardButtons: Dialog.Ok


        Overlay.modal: Rectangle {
            color: "#000000"
            opacity: 0.5
        }
    }
}
