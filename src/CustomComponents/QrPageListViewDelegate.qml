import QtQuick 2.15
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Controls.Material.impl

RowLayout {
    id: rowLayout
    anchors.left: parent !== null ? parent.left : undefined
    anchors.right: parent !== null ? parent.right : undefined
    anchors.rightMargin: 0
    anchors.leftMargin: 0
    spacing: 0

    property string question : ""
    property string response : ""

    Rectangle {
        id: qBox
        height: Math.max(qText.contentHeight,
                         rText.contentHeight) + 20
        color: "#00ffffff"
        border.color: "#49454f"
        border.width: 1
        Layout.fillWidth: true
        Layout.fillHeight: true

        PrimaryText {
            id: qText
            text: question
            anchors.fill: parent
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 8
            anchors.leftMargin: 8
            anchors.bottomMargin: 8
            anchors.topMargin: 8
        }
    }

    Rectangle {
        id: rBox
        color: "#00ffffff"
        height: Math.max(qText.contentHeight,
                         rText.contentHeight) + 20
        border.width: 1
        Layout.fillHeight: true
        border.color: "#49454f"
        Layout.fillWidth: true

        PrimaryText {
            id: rText
            text: response
            anchors.fill: parent

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 8
            anchors.rightMargin: 8
            anchors.leftMargin: 8
            anchors.bottomMargin: 8
        }
    }

    onVisibleChanged: {
        height = Math.max(qText.contentHeight, rText.contentHeight) + 20;
    }

    Component.onCompleted: {
        height = Math.max(qText.contentHeight, rText.contentHeight) + 20;
    }
}
