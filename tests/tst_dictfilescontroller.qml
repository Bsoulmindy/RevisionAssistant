import QtQuick 2.15
import QtTest 1.0
import RevisionAssistant

TestCase {
    name: "dictfilescontroller_tests"

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

    DictFilesController {
        id: dictFilesController_basic
        dict_controller: dictController_basic

        Component.onCompleted: {
            init()
        }
    }

    function cleanup() {
        if(dictFilesController_basic.selected_file === "test_basic_dict1.json") {
            dictFilesController_basic.rename_database_file("test_basic_dict");
        }

        dictFilesController_basic.select_file("test_basic_dict.json");
        dictFilesController_basic.use_database_file("test_basic_dict.json");
        dictFilesController_basic.mark_default_database_file("test_basic_dict.json");
        dictFilesController_basic.error.connect((msg) => {
            // reset
        });
    }

    function test_get_file_name_without_extension_data() {
        return [
            {tag: "without folder", input: "hello.json", output: "hello" },
            {tag: "with folders", input: "/folder1/folder2/hello.json", output: "hello" },
            {tag: "without extension", input: "/folder1/folder2/abc", output: "abc" },
            {tag: "folder have . in its name", input: "/folder1/folder.2/abcd", output: "abcd" },
        ]
    }

    function test_get_file_name_without_extension(data) {
        compare(dictFilesController_basic.get_file_name_without_extension(data.input), data.output, data.tag);
    }

    function test_select_file_from_list() {
        let files = dictFilesController_basic.files;
        verify(files.length >= 3, "Should detect the empty, basic and large dicts");
        dictFilesController_basic.select_file("test_large_dict.json");
        verify(dictFilesController_basic.selected_file === "test_large_dict.json", "Should select the large dict");
    }

    function test_create_and_remove_dict() {
        dictFilesController_basic.add_new_empty_file("test_dictfilescontroller", 0);
        verify(dictFilesController_basic.files.length >= 4, "Should detect the 3 autocreated dicts + the new created dict");
        dictFilesController_basic.use_database_file("test_dictfilescontroller.json");
        compare(dictFilesController_basic.current_file, "test_dictfilescontroller.json", "Should used the new created dict");

        // Remove a dict that is currently being used
        let isErrorThrown = false;
        dictFilesController_basic.error.connect((msg) => {
            isErrorThrown = true;
        });
        dictFilesController_basic.remove_database_file("test_dictfilescontroller.json")
        compare(isErrorThrown, true, "The files controller should not remove a dict that is being used");

        // Returning back to basic dict
        dictFilesController_basic.use_database_file("test_basic_dict.json");
        compare(dictFilesController_basic.current_file, "test_basic_dict.json", "Should used bach the basic dict");

        // Make the newly created dict default, and try to delete the default dict
        dictFilesController_basic.mark_default_database_file("test_dictfilescontroller.json");
        compare(dictFilesController_basic.default_file, "test_dictfilescontroller.json", "The default dict should have been changed!");
        isErrorThrown = false;
        dictFilesController_basic.remove_database_file("test_dictfilescontroller.json")
        compare(isErrorThrown, true, "The files controller should not remove a default dict");

        // Make basic dict default again
        dictFilesController_basic.mark_default_database_file("test_basic_dict.json");
        compare(dictFilesController_basic.default_file, "test_basic_dict.json", "The default dict should have been changed!");

        // Delete the created dict
        dictFilesController_basic.remove_database_file("test_dictfilescontroller.json")
        verify(dictFilesController_basic.files.length >= 3, "Should detect the 3 autocreated dicts");
    }

    function test_rename_dict() {
        dictFilesController_basic.error.connect((msg) => {
            fail("Should not return an error : " + msg);
        });

        dictFilesController_basic.select_file("test_basic_dict.json");
        dictFilesController_basic.rename_database_file("test_basic_dict1");
        compare(dictFilesController_basic.selected_file, "test_basic_dict1.json", "The selected file name should also be renamed");
        compare(dictFilesController_basic.default_file, "test_basic_dict1.json", "The selected file name should also be renamed");
        compare(dictFilesController_basic.current_file, "test_basic_dict1.json", "The selected file name should also be renamed");
    }

    function test_export_and_import() {
        dictFilesController_basic.error.connect((msg) => {
            fail("Should not return an error : " + msg);
        });

        dictFilesController_basic.select_file("test_basic_dict.json");
        let originalLen = dictFilesController_basic.dict_controller.num_rows;

        dictFilesController_basic.export_database_file(true);
        verify(fileSystemUtils.is_file_exist_from_standard("test_basic_dict.json.export"), "The export file is not found");

        dictFilesController_basic.import_database_file(true);
        verify(fileSystemUtils.is_file_exist_from_standard("mock.json"), "The imported file is not found");
        dictFilesController_basic.use_database_file("mock.json");
        compare(originalLen, dictFilesController_basic.dict_controller.num_rows, "The imported file doesn't look the same before export");
    }
}
