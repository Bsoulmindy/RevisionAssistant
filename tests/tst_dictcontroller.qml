import QtQuick 2.15
import QtTest 1.0
import RevisionAssistant

TestCase {
    name: "dictcontroller_tests"

    DictController {
        id: dictController_empty
        dict_file_name: "test_empty"
        onError: (message) => {
            if(message === "") {
                message = "Unknown error occured! Please restart the app!";
            }
            errorDialog.dialogText.text = message;
            errorDialog.open();
        }
    }

    function test_case1() {
        verify(false, "Should be initially empty");
    }
}
