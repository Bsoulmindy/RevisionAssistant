import QtQuick 2.15
import QtQuick.Controls 2.15

import RevisionAssistant
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import QtQuick.Dialogs
import QtCore
import CustomComponents

Page {
    id: dictsPage
    width: 400
    height: 800
    z: 0

    property alias backButtonItem: backButton
    property alias optionsListItem: optionsList
    property alias useButtonItem: useButton
    property alias newButtonItem: newButton
    property alias removeButtonItem: removeButton
    property alias defaultButtonItem: defaultButton
    property alias importButtonItem: importButton
    property alias exportButtonItem: exportButton
    property alias renameButtonItem: renameButton

    ToolBar {
        id: dictsPageToolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        RowLayout {
            x: 16
            y: 0
            ToolButton {
                id: backButton
                text: qsTr("")
                icon.source: "qrc:/icons/back.png"
                display: AbstractButton.IconOnly
            }

            PrimaryText {
                id: titlePage
                text: qsTr("Dictionnaries")
                font.pixelSize: 20
                verticalAlignment: Text.AlignVCenter
                color: Material.primaryTextColor
            }
        }
    }

    Rectangle {
        id: optionsListRect
        height: buttonsColumn.height
        color: "#333333"
        radius: 30
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: buttonsColumn.left
        anchors.rightMargin: 10
        anchors.leftMargin: 30
        ListView {
            id: optionsList
            anchors.fill: parent
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            model: ListModel {
                ListElement {
                    name: "Default (Current)"
                    isSelected: true
                }

                ListElement {
                    name: "Option 1"
                    isSelected: false
                }

                ListElement {
                    name: "Option 2"
                    isSelected: false
                }

                ListElement {
                    name: "Option 3"
                    isSelected: false
                }
            }
            delegate: FileOptionListDelegate {
                modelDb: modelData
                width: optionsList.width
                height: 35
            }

            anchors.topMargin: 20
            anchors.rightMargin: 20
            anchors.leftMargin: 20
            anchors.bottomMargin: 20
        }
        Layout.preferredHeight: 272
        Layout.fillWidth: true
    }

    ColumnLayout {
        id: buttonsColumn
        x: 2
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 30
        spacing: 0

        Button {
            id: newButton
            text: qsTr("New")
            Layout.preferredWidth: 117
            Layout.preferredHeight: 52
            Layout.fillWidth: true
        }

        Button {
            id: renameButton
            text: qsTr("Rename")
            Layout.preferredWidth: 117
            Layout.preferredHeight: 52
            Layout.fillWidth: true
        }

        Button {
            id: useButton
            text: qsTr("Use")
            Layout.preferredWidth: 117
            Layout.preferredHeight: 52
            Layout.fillWidth: true
            Layout.fillHeight: false
        }

        Button {
            id: defaultButton
            text: qsTr("Default")
            Layout.preferredWidth: 117
            Layout.preferredHeight: 52
            Layout.fillWidth: true
        }

        Button {
            id: removeButton
            text: qsTr("Remove")
            Layout.preferredWidth: 117
            Layout.preferredHeight: 52
            Layout.fillWidth: true
        }

        Button {
            id: importButton
            text: qsTr("Import")
            Layout.preferredWidth: 117
            Layout.preferredHeight: 52
            Layout.fillWidth: true
        }

        Button {
            id: exportButton
            text: qsTr("Export")
            Layout.preferredWidth: 117
            Layout.preferredHeight: 52
            Layout.fillWidth: true
        }
    }
}
