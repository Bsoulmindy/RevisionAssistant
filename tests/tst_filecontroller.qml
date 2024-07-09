import QtQuick 2.15
import QtTest 1.0
import RevisionAssistant

TestCase {
    name: "filecontroller_tests"

    FileSystemUtils {
        id: fileSystemUtils
    }

    DictController {
        id: dictController_basic
        dict_file_name: "test_basic_dict"
        Component.onCompleted: {
            init()
        }
    }

    FileController {
        id: fileController_basic
        dict_controller: dictController_basic
        onError: (message) => {
            fail("Error : " + message);
        }
    }

    DictController {
        id: dictController_MToM_basic
        dict_file_name: "test_basic_MToM_dict"
        Component.onCompleted: {
            init()
        }
    }

    FileController {
        id: fileController_MToM_basic
        dict_controller: dictController_MToM_basic
        onError: (message) => {
            fail("Error : " + message);
        }
    }

    function test_construct_from_valid_file() {
        let nbWarnings = 0;
        fileController_basic.warningOutput.connect((output) => {
            nbWarnings++;
        });
        fileController_basic.constructDictFromFile("file:" + fileSystemUtils.get_dir() + "test_mapper_basic.txt", "|");
        compare(nbWarnings, 0, "A valid file shouldn't have any warnings");
        compare(fileController_basic.dict_controller.num_rows, 10, "The valid file should construct 10 entries correctly");

        nbWarnings = 0;
        fileController_MToM_basic.warningOutput.connect((output) => {
            nbWarnings++;
        });
        fileController_MToM_basic.constructMToMDictFromFile("file:" + fileSystemUtils.get_dir() + "test_mapper_basic.txt", "|", "/");
        compare(nbWarnings, 0, "A valid file shouldn't have any warnings");
        compare(fileController_MToM_basic.dict_controller.num_rows, 20, "The valid file should construct 10 entries correctly");
    }

    function test_construct_from_3parts_case() {
        let nbWarnings = 0;
        fileController_basic.warningOutput.connect((output) => {
            nbWarnings++;
        });
        fileController_basic.constructDictFromFile("file:" + fileSystemUtils.get_dir() + "test_mapper_3parts.txt", "|");
        compare(nbWarnings, 10, "On 3 parts, the file should have warning for each line");
        compare(fileController_basic.dict_controller.num_rows, 10, "On 3 parts, the file controller should construct 10 entries whatsoever");

        nbWarnings = 0;
        fileController_MToM_basic.warningOutput.connect((output) => {
            nbWarnings++;
        });
        fileController_MToM_basic.constructMToMDictFromFile("file:" + fileSystemUtils.get_dir() + "test_mapper_3parts.txt", "|", "/");
        compare(nbWarnings, 10, "On 3 parts, the file should have warning for each line");
        compare(fileController_MToM_basic.dict_controller.num_rows, 20, "On 3 parts, the file controller should construct 10 entries whatsoever");
    }

    function test_construct_from_1part_case() {
        let nbWarnings = 0;
        fileController_basic.warningOutput.connect((output) => {
            nbWarnings++;
        });
        fileController_basic.constructDictFromFile("file:" + fileSystemUtils.get_dir() + "test_mapper_1part.txt", "|");
        compare(nbWarnings, 10, "On 1 part case, the file should have warning for each line");
        compare(fileController_basic.dict_controller.num_rows, 0, "On 1 part case, the file controller should not construct any entry since we must have 1 question and 1 response");

        nbWarnings = 0;
        fileController_MToM_basic.warningOutput.connect((output) => {
            nbWarnings++;
        });
        fileController_MToM_basic.constructMToMDictFromFile("file:" + fileSystemUtils.get_dir() + "test_mapper_1part.txt", "|", "/");
        compare(nbWarnings, 10, "On 1 part case, the file should have warning for each line");
        compare(fileController_MToM_basic.dict_controller.num_rows, 0, "On 1 part case, the file controller should not construct any entry since we must have 1 question and 1 response");
    }

    function test_construct_from_empty_case() {
        let nbWarnings = 0;
        fileController_basic.warningOutput.connect((output) => {
            nbWarnings++;
        });
        fileController_basic.constructDictFromFile("file:" + fileSystemUtils.get_dir() + "test_mapper_empty.txt", "|");
        compare(nbWarnings, 10, "On empty case (0 part), the file should have warning for each line");
        compare(fileController_basic.dict_controller.num_rows, 0, "On empty case (0 part), the file controller should not construct any entry since we must have 1 question and 1 response");

        nbWarnings = 0;
        fileController_MToM_basic.warningOutput.connect((output) => {
            nbWarnings++;
        });
        fileController_MToM_basic.constructMToMDictFromFile("file:" + fileSystemUtils.get_dir() + "test_mapper_empty.txt", "|", "/");
        compare(nbWarnings, 10, "On empty case (0 part), the file should have warning for each line");
        compare(fileController_MToM_basic.dict_controller.num_rows, 0, "On empty case (0 part), the file controller should not construct any entry since we must have 1 question and 1 response");
    }
}
