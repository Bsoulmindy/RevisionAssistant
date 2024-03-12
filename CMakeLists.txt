cmake_minimum_required(VERSION 3.16)

message(DEBUG "QT_HOST_PATH is : " ${QT_HOST_PATH})

project(RevisionAssistant VERSION 0.1.0 LANGUAGES CXX)

# configuration file
configure_file(config/config.h.in ${CMAKE_SOURCE_DIR}/config/config.h)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
# recognize the app as executable if compiled with GCC Linux
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpie -O3")
if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fwasm-exceptions")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fwasm-exceptions")
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Core Widgets)

qt_standard_project_setup(REQUIRES 6.5)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/config/resource.rc")
if(WIN32)
    qt_add_executable(RevisionAssistantApp
	src/main.cpp
	${app_icon_resource_windows}
    )
else()
    qt_add_executable(RevisionAssistantApp
	src/main.cpp
    )
endif()

include_directories(src/controllers)

qt_add_qml_module(RevisionAssistantApp
    URI RevisionAssistant
    VERSION 1.0
    QML_FILES
        src/Main.qml
        src/views/DictEditPage.qml
        src/views/DictEditPageUi.ui.qml
        src/views/MainPage.qml
        src/views/MainPageUi.ui.qml
        src/views/QrPage.qml
        src/views/QrPageUi.ui.qml
        src/views/DictAddPage.qml
        src/views/DictAddPageUi.ui.qml
        src/views/DictProcessingPage.qml
        src/views/DictProcessingPageUi.ui.qml
    RESOURCES resources.qrc
    SOURCES src/controllers/dictcontroller.h src/controllers/dictcontroller.cpp
            src/controllers/quizcontroller.h src/controllers/quizcontroller.cpp
            src/controllers/filecontroller.h src/controllers/filecontroller.cpp
            src/models/question_response_entry.h src/models/question_response_entry.cpp
            src/repositories/dict_repo_interface.h
            src/repositories/dict_json_repo.h src/repositories/dict_json_repo.cpp
            src/exceptions/json_exception.h src/exceptions/json_exception.cpp
            src/exceptions/file_invalid_json_exception.h src/exceptions/file_invalid_json_exception.cpp
            src/exceptions/object_invalid_json_exception.h src/exceptions/object_invalid_json_exception.cpp
            src/factories/dict_repo_factory.h src/factories/dict_repo_factory.cpp
            src/models/dict_repo_enum.h
            src/models/question_response_entries_set.h src/models/question_response_entries_set.cpp
	    src/exceptions/repo_exception.h
	    src/exceptions/run_out_of_entries.h src/exceptions/run_out_of_entries.cpp
	    SOURCES src/controllers/fileselectorcontroller.h src/controllers/fileselectorcontroller.cpp

)

add_subdirectory(src/CustomComponents)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(RevisionAssistantApp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.RevisionAssistantApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

target_link_libraries(RevisionAssistantApp
    PRIVATE Qt6::Quick Qt6::Core Qt6::Widgets custom_componentsplugin
)
if(EMSCRIPTEN)
    target_link_libraries(RevisionAssistantApp PRIVATE idbfs.js)
    target_link_options(RevisionAssistantApp PRIVATE "SHELL:-s FORCE_FILESYSTEM=1 ")
endif()

############################################# INSTALL
qt_standard_project_setup()

install(TARGETS RevisionAssistantApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET RevisionAssistantApp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

list(APPEND QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/src")
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE PATH
    "Path to the custom QML components defined by the project" FORCE)
