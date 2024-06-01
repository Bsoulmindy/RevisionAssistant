import QtQuick 2.15
import QtTest 1.0
import RevisionAssistant

TestCase {
    name: "fileselectorcontroller_tests"

    FileSelectorController {
        id: fileSelectorController
    }

    function test_open_mapper_file() {
        fileSelectorController.openFile("|", true);
        let bytes = fileSelectorController.getFileBytes();
        verify(bytes.byteLength > 0, "Should read bytes of a mapper file without any problems");
    }
}
