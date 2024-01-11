import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import RevisionAssistant
import "components"

Page {
    id: dictEditPage
    width: root.width
    height: root.height

    z: 0

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
                text: qsTr("Edit Dictionnary")
                font.pixelSize: 20
                verticalAlignment: Text.AlignVCenter
                color: Material.primaryTextColor
            }
        }
    }

    Rectangle {
        id: dictBox
        color: "#333333"
        radius: 30
        border.width: 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchBox.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 30
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        anchors.bottomMargin: 30

        ListView {
            id: listView

            property string searchText: ""

            anchors.fill: parent
            anchors.rightMargin: 20
            anchors.leftMargin: 20
            anchors.bottomMargin: 20
            anchors.topMargin: 20
            spacing: 0
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {}
            model: dictController.getAllRecords()
            delegate: RowLayout {
                property bool isVisible: (listView.searchText === "" || modelData.question.includes(listView.searchText) || modelData.response.includes(listView.searchText))
                id: rowDelegate
                anchors.left: parent !== null ? parent.left : undefined
                anchors.right: parent !== null ? parent.right : undefined
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                spacing: 0
                visible: isVisible

                onVisibleChanged: {
                    rowDelegate.height = isVisible ? Math.max(qText.contentHeight, rText.contentHeight, qCheckBox.implicitHeight - 20) + 20 : 0;
                }

                Component.onCompleted: {
                    rowDelegate.height = isVisible ? Math.max(qText.contentHeight, rText.contentHeight, qCheckBox.implicitHeight - 20) + 20 : 0;
                }

                Rectangle {
                    id: qBox
                    height: parent.height
                    color: "#00ffffff"
                    border.color: "#49454f"
                    border.width: 1
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    CheckBox {
                        id: qCheckBox
                        text: qsTr("")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 0
                        anchors.topMargin: 0
                        checkState: modelData.isCheckedQuestion ? Qt.Checked : Qt.Unchecked

                        onCheckedChanged: {
                            if(checked) {
                                dictController.checkQuestionInDatabase(modelData.id)
                            } else {
                                dictController.uncheckQuestionInDatabase(modelData.id)
                            }
                        }
                    }

                    PrimaryText {
                        id: qText
                        text: modelData.question
                        anchors.left: qCheckBox.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                        anchors.bottomMargin: 0
                        anchors.topMargin: 0
                        anchors.leftMargin: 5
                        anchors.rightMargin: 5
                    }
                }

                Rectangle {
                    id: rBox
                    color: "#00ffffff"
                    height: parent.height
                    border.width: 1
                    Layout.fillHeight: true
                    border.color: "#49454f"
                    Layout.fillWidth: true

                    CheckBox {
                        id: rCheckBox
                        text: qsTr("")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 0
                        anchors.topMargin: 0
                        checkState: modelData.isCheckedResponse ? Qt.Checked : Qt.Unchecked

                        onCheckedChanged: {
                            if(checked) {
                                dictController.checkResponseInDatabase(modelData.id)
                            } else {
                                dictController.uncheckResponseInDatabase(modelData.id)
                            }
                        }
                    }

                    PrimaryText {
                        id: rText
                        text: modelData.response
                        anchors.left: rCheckBox.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.bottomMargin: 0
                        anchors.topMargin: 0
                        anchors.leftMargin: 5
                        anchors.rightMargin: 5
                    }
                }
            }
        }
    }

    Rectangle {
        id: searchBox
        height: 62
        color: "#00333333"
        radius: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: dictEditPageToolBar.bottom
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        anchors.topMargin: 30

        RowLayout {
            anchors.fill: parent
            anchors.rightMargin: 10
            anchors.leftMargin: 20
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            spacing: 0
            TextField {
                id: searchTextField
                font.pixelSize: Math.max(14, Math.min(20,
                                                      8 + dictEditPage.width / 100))
                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                placeholderText: qsTr("Search")
                Layout.preferredWidth: 202
                Layout.preferredHeight: 34
                onAccepted: {
                    listView.searchText = searchTextField.text
                }
            }

            RoundButton {
                id: searchButton
                implicitHeight: parent.height
                implicitWidth: parent.height
                width: parent.height
                height: parent.height
                text: ""
                icon.source: "qrc:/icons/search.png"
                display: AbstractButton.IconOnly
                Material.background: "#00000000"

                onClicked: {
                    listView.searchText = searchTextField.text
                }
            }
        }
    }

    StackView.onRemoved: {
        dictController.init()
    }
}
