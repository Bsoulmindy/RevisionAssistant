import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import RevisionAssistant

Page {
    id: qrPage
    width: root.width
    height: root.height

    z: 0
    required property bool isQtoR

    QuizController {
        id: quizController
        dict_controller: dictController
        isQtoR: qrPage.isQtoR
        onOutputDismissed: (output) => {
            listView.model.append({ "question": output.question, "response": output.response })
            listView.scrollToEnd()
        }
        Component.onCompleted: {
            init()
        }
    }

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
                icon.source: "qrc:/icons/back.png"
                display: AbstractButton.IconOnly
                onClicked: stackView.pop()
            }

            Text {
                id: titlePage
                text: isQtoR ? qsTr("Question To Response") : qsTr("Response To Question")
                font.pixelSize: 20
                verticalAlignment: Text.AlignVCenter
                color: Material.primaryTextColor
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
            model: ListModel {}

            NumberAnimation { id: anim; target: listView; property: "contentY"; duration: 500; easing.type: Easing.InOutQuad }

            function scrollToEnd() {
                anim.running = false;

                var pos = listView.contentY;
                var destPos;

                listView.positionViewAtEnd();
                destPos = listView.contentY;

                anim.from = pos;
                anim.to = destPos;
                anim.running = true;
            }

            delegate: RowLayout {
                height: Math.max(qText.contentHeight,
                                 rText.contentHeight) + 20
                anchors.left: parent !== null ? parent.left : 0
                anchors.right: parent !== null ? parent.right : 0
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                spacing: 0
                Rectangle {
                    id: qBox
                    height: Math.max(qText.contentHeight,
                                     rText.contentHeight) + 20
                    color: "#00ffffff"
                    border.color: "#49454f"
                    border.width: 1
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Text {
                        id: qText
                        text: model.question
                        anchors.fill: parent
                        font.pixelSize: Math.max(14, Math.min(
                                                     20,
                                                     8 + qrPage.width / 100))
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                        anchors.rightMargin: 8
                        anchors.leftMargin: 8
                        anchors.bottomMargin: 8
                        anchors.topMargin: 8
                        color: Material.primaryTextColor
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

                    Text {
                        id: rText
                        text: model.response
                        anchors.fill: parent
                        font.pixelSize: Math.max(14, Math.min(
                                                     20,
                                                     8 + qrPage.width / 100))
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                        anchors.topMargin: 8
                        anchors.rightMargin: 8
                        anchors.leftMargin: 8
                        anchors.bottomMargin: 8
                        color: Material.primaryTextColor
                    }
                }
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
            id: completeButton

            Connections {
                target: completeButton

                function onClicked() {
                    if(quizController.current_output["isChecked"]) {
                        quizController.unmark_output();
                    } else {
                        quizController.mark_output();
                    }

                    completeButton.state = quizController.current_output["isChecked"] ? "completed" : "";
                }
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

                Text {
                    id: outputText
                    text: isQtoR ? quizController.current_output["question"] : quizController.current_output["response"]
                    anchors.verticalCenter: parent.verticalCenter

                    font.pixelSize: Math.max(14,
                                             Math.min(20,
                                                      8 + qrPage.width / 100))
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter

                    color: Material.primaryTextColor
                    transform: textTranslate

                    Translate {
                        id: textTranslate
                    }

                    Behavior on text {
                        SequentialAnimation {
                            PropertyAction { target: textTranslate; property: "y"; value: 0 }
                            ParallelAnimation {
                                NumberAnimation {
                                    target: textTranslate
                                    property: "y"
                                    from: 0
                                    to: -20
                                    duration: 250
                                    easing.type: Easing.InQuad
                                }
                                NumberAnimation {
                                    target: outputText
                                    property: "opacity"
                                    to: 0
                                    easing.type: Easing.InQuad
                                    duration: 250
                                }
                            }
                            PropertyAction { }
                            ParallelAnimation {
                                NumberAnimation {
                                    target: textTranslate
                                    property: "y"
                                    from: 20
                                    to: 0
                                    duration: 250
                                    easing.type: Easing.OutQuad
                                }
                                NumberAnimation {
                                    target: outputText
                                    property: "opacity"
                                    to: 1
                                    easing.type: Easing.OutQuad
                                    duration: 250
                                }
                            }
                        }
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
            onClicked: quizController.next_output()
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
                Text {
                    id: progressTextDesc
                    color: Material.primaryTextColor
                    text: qsTr("Your progress : ")
                    font.pixelSize: Math.max(14,
                                             Math.min(20,
                                                      8 + qrPage.width / 100))
                    Layout.fillWidth: true
                }

                Text {
                    id: progressTextPercent
                    color: Material.primaryTextColor
                    text: quizController.progression.toFixed(2) * 100 + " %"
                    font.pixelSize: Math.max(14,
                                             Math.min(20,
                                                      8 + qrPage.width / 100))
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }
            }

            ProgressBar {
                id: progressRevisionBar
                Layout.fillWidth: true
                value: quizController.progression
                Layout.preferredHeight: 6

                property color valueColor: calculateColor(value)

                Behavior on value {
                    NumberAnimation {
                        duration: 500
                        easing.type: Easing.InOutQuad
                    }
                }

                function calculateColor(value) {
                    var r, g, b;

                    if (value < 0.5) {
                        // from red (1,0,0) to yellow (1,1,0)
                        r = 1;
                        g = value * 2;
                        b = 0;
                    } else {
                        // from yellow (1,1,0) to green (0,1,0)
                        r = 2 * (1 - value);
                        g = 1;
                        b = 0;
                    }

                    return Qt.rgba(r, g, b, 1);
                }

                contentItem: ProgressBarImpl {
                    implicitHeight: 6

                    scale: progressRevisionBar.mirrored ? -1 : 1
                    color: progressRevisionBar.valueColor
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
