

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import CustomComponents

Item {
    width: 24
    height: 24
    Material.background: "#00000000"
    states: [
        State {
            name: "completed"
            PropertyChanges {
                target: backgroundRect
                visible: true
            }
        }
    ]

    Rectangle {
        id: backgroundRect
        width: 24
        height: 24
        color: "#0B8400"
        radius: 12
        visible: false

        PrimaryText {
            id: iconText
            text: "\u2713"
            anchors.centerIn: parent
            font.pixelSize: 22
            color: "white"
        }
    }
}
