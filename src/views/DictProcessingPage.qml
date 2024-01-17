import QtQuick 2.15
import QtQuick.Controls 2.15

import RevisionAssistant
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import CustomComponents

Page {
    id: dictProcessingPage

    required property string filePath
    required property string fileSeparator

    FileController {
        id: fileController
        dict_controller: dictController
        onWarningOutput: {
            listView.model.append({ "output": output });
        }
        onInitialized: {
            fileController.constructDictFromFile(filePath, fileSeparator)
        }
    }

    ToolBar {
        id: dictProcessingPageToolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        RowLayout {
            x: 16
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.topMargin: 0
            ToolButton {
                id: backButton
                opacity: 0
                visible: true
                text: qsTr("")
                icon.source: "qrc:/icons/back.png"
                display: AbstractButton.IconOnly
            }

            PrimaryText {
                id: titlePage
                text: qsTr("Processing the File")
                font.pixelSize: 20
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    Button {
        id: closeButton
        y: 715
        width: 123
        height: 52
        text: qsTr("Close")
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 30
        font.pointSize: 12
        Material.background: Material.primaryColor
        Layout.preferredWidth: 105
        Layout.preferredHeight: 52
        Layout.fillWidth: false
        enabled: fileController.actualState === "Finished"
        onClicked: stackView.pop()
    }

    ColumnLayout {
        id: statusColumn
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: dictProcessingPageToolBar.bottom
        anchors.topMargin: 30
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        PrimaryText {
            id: statusText
            text: fileController.actualState
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        ProgressBar {
            id: progressBar
            Layout.fillWidth: true
            value: fileController.actualState === "Finished" ? 1 : 0
            indeterminate: fileController.actualState !== "Idle" && fileController.actualState !== "Finished"
        }
    }

    ColumnLayout {
        id: consoleColumn
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: statusColumn.bottom
        anchors.bottom: closeButton.top
        anchors.bottomMargin: 50
        anchors.topMargin: 50
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        RowLayout {
            spacing: 10
            IconLabel {
                icon.width: 30
                icon.source: "qrc:/icons/warning.png"
                icon.height: 30
                icon.color: Material.primaryTextColor
            }

            PrimaryText {
                id: consoleLegend
                text: qsTr("Warnings")
                font.pixelSize: 30
            }
        }

        Rectangle {
            id: historyBox
            color: "#333333"
            radius: 30
            border.width: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            ListView {
                id: listView
                anchors.fill: parent
                spacing: 0
                model: ListModel {}
                delegate: RowLayout {
                    height: rowConsole.height
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 5
                    IconLabel {
                        icon.width: 24
                        icon.source: "qrc:/icons/attention_mark.png"
                        icon.height: 24
                        icon.color: Material.primaryTextColor
                        Layout.preferredWidth: 24
                        Layout.preferredHeight: 24
                    }

                    Rectangle {
                        id: rowConsole
                        height: qText.contentHeight + 20
                        color: "#00ffffff"
                        PrimaryText {
                            id: qText
                            text: model.output
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            anchors.topMargin: 8
                            anchors.rightMargin: 8
                            anchors.leftMargin: 8
                            anchors.bottomMargin: 8
                        }
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                }
                clip: true
                anchors.topMargin: 20
                anchors.rightMargin: 20
                anchors.leftMargin: 20
                anchors.bottomMargin: 20
                ScrollBar.vertical: ScrollBar {}
            }
            Layout.preferredHeight: 300
        }
    }
}
