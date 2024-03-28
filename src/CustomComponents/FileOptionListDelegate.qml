import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Controls.Material.impl

Item {
    property var modelDb

    Rectangle {
        color: modelDb["isSelected"] ? Material.accentColor : "transparent"
        anchors.fill: parent
        PrimaryText {
            id: optionText
            text: modelDb["name"]
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                dictFilesController.select_file(modelDb["real_name"])
            }
        }
        anchors.topMargin: 2
        anchors.bottomMargin: 2
    }
}
