import QtQuick 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts
import RevisionAssistant
import CustomComponents

Page {
    id: mainPage
    height: 800
    width: 400
    objectName: "MainPage"

    property alias dictMenuButtonItem: dictMenuButton
    property alias optionsButtonItem: optionsButton
    property alias dictMenuItem: dictMenu
    property alias optionsMenuItem: optionsMenu
    property alias aboutDialogItem: aboutDialog
    property alias menuItemOpenItem: menuItemOpen
    property alias menuItemEditItem: menuItemEdit
    property alias menuItemResetItem: menuItemReset
    property alias menuItemInfoItem: menuItemInfo
    property alias qTorButtonItem: qTorButton
    property alias rToqButtonItem: rToqButton
    property alias resetDialogItem: resetDialog
    property alias githubLinkItem: githubLink

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

                Menu {
                    id: dictMenu
                    y: dictMenuButton.height

                    MenuItem {
                        id: menuItemOpen
                        height: 52
                        text: "Open"
                        icon.source: "qrc:/icons/open_file.png"
                    }
                    MenuItem {
                        id: menuItemEdit
                        height: 52
                        text: "Edit"
                        icon.source: "qrc:/icons/edit.png"
                    }
                    MenuItem {
                        id: menuItemReset
                        text: "Reset"
                        height: 52
                        icon.source: "qrc:/icons/reset.png"
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

                Menu {
                    id: optionsMenu
                    y: dictMenuButton.height

                    MenuItem {
                        id: menuItemInfo
                        text: "About"
                        height: 52
                        icon.source: "qrc:/icons/info.png"
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
            font.family: globalFont.font.family
            Layout.preferredWidth: 205
            Layout.preferredHeight: 52
            Material.background: Material.primaryColor
        }

        Button {
            id: rToqButton
            text: qsTr("Response To Question")
            Layout.fillWidth: true
            font.pointSize: 12
            font.family: globalFont.font.family
            Layout.preferredWidth: 205
            Layout.preferredHeight: 52
            Material.background: Material.primaryColor
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
                        id: githubLink
                        anchors.fill: parent
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
