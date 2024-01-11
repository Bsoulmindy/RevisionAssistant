import QtQuick 2.15
import QtQuick.Controls 2.15

import RevisionAssistant
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import QtQuick.Dialogs
import QtCore
import "components"

Page {
    id: dictAddPageForm

    ToolBar {
        id: dictEditPageToolBar
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
                onClicked: stackView.pop()
            }

            PrimaryText {
                id: titlePage
                text: qsTr("Add Dictionnary")
                font.pixelSize: 20
                verticalAlignment: Text.AlignVCenter
                color: Material.primaryTextColor
            }
        }
    }

    ColumnLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20
        width: 340
        RowLayout {
            id: selectFileRow
            Layout.preferredWidth: parent.width
            spacing: 0
            PrimaryText {
                id: selectionFileText
                text: qsTr("Select File")
                font.pixelSize: 16
                Layout.preferredWidth: parent.width / 2
            }

            RowLayout {
                Button {
                    id: selectDictButton
                    text: qsTr("Select")
                    font.pointSize: 12
                    Material.background: Material.primaryColor
                    Layout.preferredWidth: 105
                    Layout.preferredHeight: 52
                    Layout.fillWidth: false
                    onClicked: fileDialog.open()
                }

                FileSelectionStatus {
                    id: fileSelectionStatus
                }
                Layout.preferredWidth: parent.width / 2
            }
        }

        RowLayout {
            id: selectSeparatorRow
            spacing: 0
            Layout.preferredWidth: parent.width
            RowLayout {
                Layout.preferredWidth: parent.width / 2
                PrimaryText {
                    id: separatorText
                    text: qsTr("Select Separator")
                    font.pixelSize: 16
                    Layout.fillWidth: false
                }

                RoundButton {
                    id: selectSeparatorHelp
                    text: "?"
                    Layout.fillWidth: false
                    font.pointSize: 10
                    radius: 20
                    Layout.preferredHeight: 29
                    Layout.preferredWidth: 29
                    onClicked: separatorHelpDialog.open()

                    background: Rectangle {
                        implicitWidth: Material.buttonHeight
                        implicitHeight: Material.buttonHeight

                        radius: selectSeparatorHelp.radius
                        color: "#00000000"
                        border.color: Material.primaryTextColor
                        border.width: 2

                        Rectangle {
                            width: parent.width
                            height: parent.height
                            radius: selectSeparatorHelp.radius
                            visible: enabled
                                     && (selectSeparatorHelp.hovered
                                         || selectSeparatorHelp.visualFocus)
                            color: Material.rippleColor
                        }

                        Rectangle {
                            width: parent.width
                            height: parent.height
                            radius: selectSeparatorHelp.radius
                            visible: selectSeparatorHelp.down
                            color: Material.rippleColor
                        }

                        layer.enabled: selectSeparatorHelp.enabled
                                       && color.a > 0
                                       && !selectSeparatorHelp.flat
                        layer.effect: ElevationEffect {
                            elevation: Material.elevation
                        }
                    }
                }
            }

            TextField {
                id: searchTextField
                font.pixelSize: 20
                verticalAlignment: Text.AlignVCenter
                Layout.preferredWidth: parent.width / 2
                Layout.preferredHeight: parent.height
            }

            Layout.preferredHeight: 40
        }

        Button {
            id: addDictButton
            text: qsTr("Add")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            font.pointSize: 12
            Material.background: Material.primaryColor
            Layout.preferredWidth: 105
            Layout.preferredHeight: 52
            onClicked: {
                if (fileDialog.selectedFile === "" || searchTextField.text === "") {
                    invalidFileDialog.open()
                } else {
                    stackView.push("DictProcessingPage.qml", {"filePath": fileDialog.selectedFile, "fileSeparator": searchTextField.text})
                }
            }

        }
    }

    Dialog {
        id: separatorHelpDialog
        title: "What is a separator?"
        anchors.centerIn: parent
        width: 350
        height: 250
        contentItem: Item {
            PrimaryText {
                height: contentHeight
                text: "A separator is a character or list of characters to differentiate between the question and the response"
                anchors.fill: parent
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.Wrap
                anchors.rightMargin: 20
                anchors.leftMargin: 20
                anchors.bottomMargin: 20
                anchors.topMargin: 20

                font.pixelSize: 14

                onLineLaidOut: (line)=> {
                    if (line.y + topPadding <= separator_example.y + separator_example.height) {
                        line.width = separator_example.x - anchors.leftMargin - anchors.rightMargin;
                    }
                }
            }

            Image {
                id: separator_example
                x: 180
                width: 150
                height: 50
                anchors.right: parent.right
                anchors.top: parent.top
                source: "qrc:/icons/separator_example.png"
                anchors.rightMargin: 20
                anchors.topMargin: 20
                fillMode: Image.PreserveAspectFit
            }
        }


        standardButtons: Dialog.Ok



        Overlay.modal: Rectangle {
            color: "#000000"
            opacity: 0.5
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        onAccepted: {
            fileSelectionStatus.state = "completed"
        }
    }

    Dialog {
        id: invalidFileDialog
        title: "Form Invalid"
        anchors.centerIn: parent
        width: 350
        height: 200
        contentItem: Item {
            PrimaryText {
                height: contentHeight
                text: "You must specify a valid .txt file AND a separator"
                anchors.fill: parent
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.Wrap
                anchors.rightMargin: 20
                anchors.leftMargin: 20
                anchors.bottomMargin: 20
                anchors.topMargin: 20

                font.pixelSize: 14
            }
        }


        standardButtons: Dialog.Ok


        Overlay.modal: Rectangle {
            color: "#000000"
            opacity: 0.5
        }
    }
}
