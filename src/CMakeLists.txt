qt_add_library(app STATIC)
qt_add_qml_module(app
    URI "RevisionAssistant"
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/RevisionAssistant"
    QML_FILES
        Main.qml
	views/DictEditPage.qml
	views/DictEditPageUi.ui.qml
	views/MainPage.qml
	views/MainPageUi.ui.qml
	views/QrPage.qml
	views/QrPageUi.ui.qml
	views/DictAddPage.qml
	views/DictAddPageUi.ui.qml
	views/DictProcessingPage.qml
	views/DictProcessingPageUi.ui.qml
	views/DictsPage.qml
	views/DictsPageUi.ui.qml
    RESOURCES ../resources.qrc
    SOURCES controllers/dictcontroller.h controllers/dictcontroller.cpp
            controllers/quizcontroller.h controllers/quizcontroller.cpp
	    controllers/filecontroller.h controllers/filecontroller.cpp
	    models/question_response_entry.h models/question_response_entry.cpp
	    repositories/dict_repo_interface.h
	    repositories/dict_json_repo.h repositories/dict_json_repo.cpp
	    exceptions/json_exception.h exceptions/json_exception.cpp
	    exceptions/file_invalid_json_exception.h exceptions/file_invalid_json_exception.cpp
	    exceptions/object_invalid_json_exception.h exceptions/object_invalid_json_exception.cpp
	    factories/dict_repo_factory.h factories/dict_repo_factory.cpp
	    models/dict_repo_enum.h
	    models/question_response_entries_set.h models/question_response_entries_set.cpp
	    exceptions/repo_exception.h
	    exceptions/run_out_of_entries.h exceptions/run_out_of_entries.cpp
	    SOURCES controllers/fileselectorcontroller.h controllers/fileselectorcontroller.cpp
	    SOURCES controllers/dictfilescontroller.h controllers/dictfilescontroller.cpp
	    SOURCES exceptions/fileexception.h exceptions/fileexception.cpp
	    SOURCES utils/filesystemutils.h utils/filesystemutils.cpp
)
target_link_libraries(app
    PUBLIC Qt6::Quick Qt6::Core Qt6::Widgets custom_componentsplugin
)
include_directories(controllers)
include_directories(utils)
