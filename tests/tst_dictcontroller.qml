import QtQuick 2.15
import QtTest 1.0
import RevisionAssistant

TestCase {
    name: "dictcontroller_tests"

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

    function test_search_term_when_empty() {
        var list = dictController_empty.getAllRecords("question")
        compare(list.length, 0, "Should be empty");
    }

    function test_search_term_when_numbers_low() {
        var list = dictController_basic.getAllRecords("question")
        compare(list.length, 10, "Should have 10 questions numbered from 1 to 10");
    }

    function test_search_term_when_numbers_high() {
        var list = dictController_large.getAllRecords("question")
        compare(list.length, 10000, "Should have 10000 questions numbered from 1 to 10000");
    }

    function test_check_question_when_empty() {
        dictController_empty.checkQuestion(0, -1);
    }

    function test_check_response_when_empty() {
        dictController_empty.checkResponse(0, -1);
    }

    function test_check_question_in_database() {
        dictController_large.checkQuestionInDatabase(1);
        verify(dictController_large.getAllRecords("question #2")[0]["isCheckedQuestion"], "Should be checked!");
        dictController_large.uncheckQuestionInDatabase(1);
        verify(!dictController_large.getAllRecords("question #2")[0]["isCheckedQuestion"], "Should be unchecked!");
    }

    function test_check_response_in_database() {
        dictController_large.checkResponseInDatabase(2);
        verify(dictController_large.getAllRecords("response #3")[0]["isCheckedResponse"], "Should be checked!");
        dictController_large.uncheckResponseInDatabase(2);
        verify(!dictController_large.getAllRecords("response #3")[0]["isCheckedResponse"], "Should be unchecked!");
    }

    function test_edit_entry() {
        dictController_large.editQuestionResponse(5000, "question #5010", "response #5010");
        compare(dictController_large.getAllRecords("question #5010").length, 2, "Should have 2 duplicate entries");
        compare(dictController_large.getAllRecords("question #5001").length, 0, "entry #4 should be gone");
        dictController_large.editQuestionResponse(5000, "question #5001", "response #5001");
        compare(dictController_large.getAllRecords("question #5010").length, 1, "Everything should look normal");
        compare(dictController_large.getAllRecords("question #5001").length, 1, "Everything should look normal");
    }

    function test_remove_and_insert() {
        dictController_large.removeEntry(9999);
        compare(dictController_large.getAllRecords("question").length, 9999, "We removed 1, so we should have 9999 elements");
        compare(dictController_large.getAllRecords("question #10000").length, 0, "The element #10000 should be the one erased");
        dictController_large.insertNewEntry("question #10000", "response #10000", false, false);
        compare(dictController_large.getAllRecords("question").length, 10000, "Everything should look normal");
        compare(dictController_large.getAllRecords("question #10000").length, 1, "Everything should look normal");
    }

    function test_reset_dict() {
        for(let i = 100; i < 105; i++) {
            dictController_large.checkQuestionInDatabase(i);
        }
        verify(dictController_large.getAllRecords("response #101")[0]["isCheckedQuestion"], "Should be checked");
        verify(dictController_large.getAllRecords("response #105")[0]["isCheckedQuestion"], "Should be checked");
        verify(!dictController_large.getAllRecords("response #106")[0]["isCheckedQuestion"], "Should not be checked");
        dictController_large.resetDict();
        verify(!dictController_large.getAllRecords("response #101")[0]["isCheckedQuestion"], "Should not be checked");
        verify(!dictController_large.getAllRecords("response #105")[0]["isCheckedQuestion"], "Should not be checked");
        verify(!dictController_large.getAllRecords("response #106")[0]["isCheckedQuestion"], "Should not be checked");
    }
}
