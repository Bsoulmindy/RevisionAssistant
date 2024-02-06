import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Controls.Material.impl

RowLayout {
    property bool isVisible: false
    property string question : ""
    property string response : ""
    property bool isCheckedQuestion: false
    property bool isCheckedResponse: false
    property int modelDataId: 0

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
            checkState: isCheckedQuestion ? Qt.Checked : Qt.Unchecked

            onCheckedChanged: {
                if(checked) {
                    dictController.checkQuestionInDatabase(modelDataId)
                } else {
                    dictController.uncheckQuestionInDatabase(modelDataId)
                }
            }
        }

        PrimaryText {
            id: qText
            text: question
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
            checkState: isCheckedResponse ? Qt.Checked : Qt.Unchecked

            onCheckedChanged: {
                if(checked) {
                    dictController.checkResponseInDatabase(modelDataId)
                } else {
                    dictController.uncheckResponseInDatabase(modelDataId)
                }
            }
        }

        PrimaryText {
            id: rText
            text: response
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
