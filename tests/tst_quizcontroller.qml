import QtQuick 2.15
import QtTest 1.0
import RevisionAssistant

TestCase {
    name: "quizcontroller_tests"

    DictController {
        id: dictController_empty
        dict_file_name: "test_empty_dict"
        Component.onCompleted: {
            init()
        }
    }

    DictController {
        id: dictController_basic
        dict_file_name: "test_basic_dict"
        Component.onCompleted: {
            init()
        }
    }

    DictController {
        id: dictController_large
        dict_file_name: "test_large_dict"
        Component.onCompleted: {
            init()
        }
    }

    QuizController {
        id: quizController_empty
        dict_controller: dictController_empty
        isQtoR: true
        Component.onCompleted: {
            init()
        }
    }

    QuizController {
        id: quizController_basic
        dict_controller: dictController_basic
        isQtoR: true
        Component.onCompleted: {
            init()
        }
    }

    QuizController {
        id: quizController_large
        dict_controller: dictController_large
        isQtoR: true
        Component.onCompleted: {
            init()
        }
    }

    function test_next_output_when_empty() {
        let isMarkedFinished = false;
        quizController_empty.finished.connect((msg) => {
            isMarkedFinished = true;
        });

        quizController_empty.next_output()
        verify(quizController_empty.current_output["question"] === "", "There should be no entry present");
        verify(isMarkedFinished, "Should indicate that is finished");
    }

    function test_next_output_when_normal() {
        let maxLoop = 100;
        while(quizController_basic.current_output["question"] !== "question3" && maxLoop > 0) {
            maxLoop--;
            quizController_basic.next_output()
        }

        verify(quizController_empty.current_output["question"] !== "question3", "Couldn't find question3 after 100 tries");
    }

    function test_mark_output() {
        let curr_output = quizController_basic.current_output["question"];
        quizController_basic.mark_output();
        quizController_basic.next_output();
        let maxLoop = 10;
        while(quizController_basic.current_output["question"] !== curr_output && maxLoop > 0) {
            maxLoop--;
            quizController_basic.next_output()
        }

        verify(quizController_basic.current_output["question"] !== curr_output, "We shouldn't find the marked question");
    }

    function test_unmark_output() {
        let curr_output = quizController_basic.current_output["question"];
        quizController_basic.mark_output();
        quizController_basic.unmark_output();
        quizController_basic.next_output();
        let maxLoop = 100;
        while(quizController_basic.current_output["question"] !== curr_output && maxLoop > 0) {
            maxLoop--;
            quizController_basic.next_output()
        }

        verify(quizController_basic.current_output["question"] === curr_output, "We should find back the unmarked entry");
    }

    function test_unmark_last_output() {
        let curr_output = quizController_basic.current_output["question"];
        quizController_basic.mark_output();
        quizController_basic.next_output();
        quizController_basic.unmark_last_output();
        quizController_basic.next_output();
        let maxLoop = 100;
        while(quizController_basic.current_output["question"] !== curr_output && maxLoop > 0) {
            maxLoop--;
            quizController_basic.next_output()
        }

        verify(quizController_basic.current_output["question"] === curr_output, "We should find back the unmarked entry");
    }
}
