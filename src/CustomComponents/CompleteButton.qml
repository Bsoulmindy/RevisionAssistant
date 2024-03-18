import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import RevisionAssistant

RoundButton {
    id: completeButton
    text: "\u2713"
    states: [
        State {
            name: "revert"
            PropertyChanges {
                target: completeButton
                text: "X"
                icon.source: "qrc:/icons/revert.png"
                Material.background: "#D80000"
            }
        }
    ]
    rightInset: 0
    leftInset: 0
    bottomInset: 0
    topInset: 0
    font.pointSize: 22
    Material.background: "#0B8400"
    Layout.fillHeight: true
    implicitHeight: 50
    implicitWidth: 50
    height: 50
    width: 50
    icon.source: "qrc:/icons/done.png"
    display: AbstractButton.IconOnly
}
