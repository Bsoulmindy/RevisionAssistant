import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import RevisionAssistant
import CustomComponents

Page {
    width: 400
    height: 800
    z: 0

    property alias backButtonItem: backButton
    property alias completeButtonItem: completeButton
    property alias nextButtonItem: nextButton
    property alias progressRevisionBarItem: progressRevisionBar
    property alias outputTextItem: outputText
    property alias textTranslateItem: textTranslate
    property alias listViewItem: listView
    property alias titlePageItem: titlePage
    property alias progressTextDescItem: progressTextDesc
    property alias progressTextPercentItem: progressTextPercent
    property alias animItem: anim
    property alias progressRevisionBarImplItem: progressRevisionBarImpl
    property alias revertButtonItem: revertButton

    ToolBar {
        id: qrPageToolBar
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
                icon.source: "../../images/back.png"
                display: AbstractButton.IconOnly
            }

            PrimaryText {
                id: titlePage
                text: qsTr("Question To Response")
                font.pixelSize: 20
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    Rectangle {
        id: historyBox
        color: "#333333"
        radius: 30
        border.width: 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: progressBox.bottom
        anchors.bottom: inputRow.top
        anchors.topMargin: 30
        anchors.bottomMargin: 30
        anchors.rightMargin: 30
        anchors.leftMargin: 30

        ListView {
            id: listView
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

            NumberAnimation {
                id: anim
                target: listView
                property: "contentY"
                duration: 500
                easing.type: Easing.InOutQuad
            }

            delegate: QrPageListViewDelegate {
                question: model.question
                response: model.response
            }
        }
    }

    RowLayout {
        id: inputRow
        y: 474
        height: 50
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 10
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        anchors.bottomMargin: 30

        CompleteButton {
            id: revertButton
            state: "revert"
            enabled: false

            ToolTip {
                visible: parent.hovered
                text: "Undo"
            }
        }

        CompleteButton {
            id: completeButton

            ToolTip {
                visible: parent.hovered
                text: "Mark as complete"
            }
        }

        Rectangle {
            id: outputBox
            color: "#333333"
            radius: 32.5
            Layout.fillWidth: true
            Layout.fillHeight: true

            Flickable {
                id: outputTextFlickable
                flickableDirection: Flickable.HorizontalFlick
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.leftMargin: 20
                height: parent.height
                clip: true

                PrimaryText {
                    id: outputText
                    text: ""
                    anchors.verticalCenter: parent.verticalCenter

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter

                    transform: textTranslate

                    Translate {
                        id: textTranslate
                    }
                }
            }
        }

        RoundButton {
            id: nextButton
            text: ">"
            rightInset: 0
            leftInset: 0
            bottomInset: 0
            topInset: 0
            highlighted: false
            flat: false
            font.pointSize: 22
            Material.background: Material.primaryColor
            Layout.fillHeight: true
            implicitHeight: 50
            implicitWidth: 50
            height: 50
            width: 50

            ToolTip {
                visible: parent.hovered
                text: "Skip"
            }
        }
    }

    Rectangle {
        id: progressBox
        height: progressTextDesc.contentHeight + progressRevisionBar.height + columnLayout.spacing
                + columnLayout.anchors.topMargin + columnLayout.anchors.bottomMargin
        color: "#333333"
        radius: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: qrPageToolBar.bottom
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        anchors.topMargin: 30
        clip: true

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent
            anchors.rightMargin: 20
            anchors.leftMargin: 20
            anchors.bottomMargin: 20
            anchors.topMargin: 20
            RowLayout {
                Layout.fillWidth: true
                PrimaryText {
                    id: progressTextDesc
                    text: qsTr("Your progress : ") + "(" + 17 + " / " + 34 + ")"
                    Layout.fillWidth: true
                }

                PrimaryText {
                    id: progressTextPercent
                    text: (50.0).toFixed(0) + " %"
                    horizontalAlignment: Text.AlignRight
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.minimumWidth: 50
                }
            }

            ProgressBar {
                id: progressRevisionBar
                Layout.fillWidth: true
                value: (0.5).toFixed(2)
                Layout.preferredHeight: 6

                contentItem: ProgressBarImpl {
                    id: progressRevisionBarImpl
                    implicitHeight: 6

                    scale: progressRevisionBar.mirrored ? -1 : 1
                    color: Qt.rgba(1, 1, 0, 1)
                    progress: progressRevisionBar.position
                    indeterminate: progressRevisionBar.visible
                                   && progressRevisionBar.indeterminate
                }

                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 6
                    y: (progressRevisionBar.height - height) / 2
                    height: 6

                    color: "#1c1b1f"
                }
            }
        }
    }
}
