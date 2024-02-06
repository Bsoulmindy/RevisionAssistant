import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import RevisionAssistant
import CustomComponents

Page {
    id: dictEditPage
    width: 400
    height: 800
    z: 0

    property alias backButtonItem: backButton
    property alias listViewItem: listView
    property alias searchTextFieldItem: searchTextField
    property alias searchButtonItem: searchButton

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
            model: ListModel {
                ListElement {
                    question: "question 1"
                    response: "response 1"
                }
                ListElement {
                    question: "question 2"
                    response: "response 2"
                }
                ListElement {
                    question: "very very very long question"
                    response: "very very very long response"
                }
                ListElement {
                    question: "question 4"
                    response: "response 4"
                }
            }
            delegate: DictEditListViewDelegate {
                isVisible: (listView.searchText === ""
                            || modelData.question.includes(listView.searchText)
                            || modelData.response.includes(listView.searchText))
                question: modelData.question
                response: modelData.response
                isCheckedQuestion: modelData.isCheckedQuestion
                isCheckedResponse: modelData.isCheckedResponse
                modelDataId: modelData.id
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
                font.pixelSize: Math.max(14,
                                         Math.min(20,
                                                  8 + dictEditPage.width / 100))
                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                placeholderText: qsTr("Search")
                Layout.preferredWidth: 202
                Layout.preferredHeight: 34
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
            }
        }
    }
}
