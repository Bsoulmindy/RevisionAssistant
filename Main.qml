

import QtQuick 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts
import RevisionAssistant

Window {
    id: root
    width: 400
    height: 800
    visible: true
    title: qsTr("Revision Assistant")
    color: Material.backgroundColor

    DictController {
        id: dictController
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainPage {}

        pushEnter: Transition {
            PropertyAnimation {
                property: "scale"
                from: 0.8
                to: 1.0
                duration: 200
            }
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 200
            }
        }

        pushExit: Transition {
            PropertyAnimation {
                property: "scale"
                from: 1.0
                to: 1.2
                duration: 200
            }
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 200
            }
        }

        popEnter: Transition {
            PropertyAnimation {
                property: "scale"
                from: 1.2
                to: 1.0
                duration: 200
            }
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 200
            }
        }

        popExit: Transition {
            PropertyAnimation {
                property: "scale"
                from: 1.0
                to: 0.8
                duration: 200
            }
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 200
            }
        }
    }
}
