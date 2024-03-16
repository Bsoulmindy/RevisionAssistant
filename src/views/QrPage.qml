import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import RevisionAssistant
import CustomComponents

QrPageUi {
    id: qrPage
    required property bool isQtoR
    property int not_checked_records: isQtoR ? dictController.num_not_checked_questions : dictController.num_not_checked_responses

    width: root.width
    height: root.height

    backButtonItem.onClicked: stackView.pop()
    completeButtonItem.onClicked: {
        quizController.mark_output()
        quizController.next_output()
    }
    nextButtonItem.onClicked: {
        quizController.next_output()
    }
    progressRevisionBarItem.value: ((dictController.num_rows - qrPage.not_checked_records) / dictController.num_rows).toFixed(2)

    titlePageItem.text: isQtoR ? qsTr("Question To Response") : qsTr(
                                     "Response To Question")
    backButtonItem.icon.source: "qrc:/icons/back.png"
    progressTextDescItem.text: qsTr("Your progress : ") + "("
                               + (dictController.num_rows - qrPage.not_checked_records)
                               + " / " + dictController.num_rows + ")"
    progressTextPercentItem.text: dictController.num_rows === 0 ? "0 %" : ((dictController.num_rows - qrPage.not_checked_records)
                               / dictController.num_rows * 100).toFixed(0) + " %"
    listViewItem.model: ListModel {}
    outputTextItem.text: isQtoR ? quizController.current_output["question"] : quizController.current_output["response"]

    Behavior on outputTextItem.text {
        SequentialAnimation {
            PropertyAction { target: qrPage.textTranslateItem; property: "y"; value: 0 }
            ParallelAnimation {
                NumberAnimation {
                    target: qrPage.textTranslateItem
                    property: "y"
                    from: 0
                    to: -20
                    duration: 250
                    easing.type: Easing.InQuad
                }
                NumberAnimation {
                    target: qrPage.outputTextItem
                    property: "opacity"
                    to: 0
                    easing.type: Easing.InQuad
                    duration: 250
                }
            }
            PropertyAction { }
            ParallelAnimation {
                NumberAnimation {
                    target: qrPage.textTranslateItem
                    property: "y"
                    from: 20
                    to: 0
                    duration: 250
                    easing.type: Easing.OutQuad
                }
                NumberAnimation {
                    target: qrPage.outputTextItem
                    property: "opacity"
                    to: 1
                    easing.type: Easing.OutQuad
                    duration: 250
                }
            }
        }
    }

    QuizController {
        id: quizController
        dict_controller: dictController
        isQtoR: qrPage.isQtoR
        onOutputDismissed: (output) => {
            qrPage.listViewItem.model.append({ "question": output.question, "response": output.response })
            qrPage.animItem.running = false;
            var pos = qrPage.listViewItem.contentY;
            qrPage.listViewItem.positionViewAtEnd();
            var destPos = qrPage.listViewItem.contentY;
            qrPage.animItem.from = pos;
            qrPage.animItem.to = destPos;
            qrPage.animItem.running = true;
        }
        Component.onCompleted: {
            init()
        }
        onFinished: (message) => {
            completionDialog.dialogText.text = message;
            completionDialog.open()
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

    Behavior on progressRevisionBarItem.value {
        NumberAnimation {
            duration: 500
            easing.type: Easing.InOutQuad
        }
    }

    progressRevisionBarImplItem.color: calculateColor(progressRevisionBarItem.value)

    InfoDialog {
        id: completionDialog
        standardButtons: Dialog.Ok
        onAccepted: {
            stackView.pop();
        }
    }
}
