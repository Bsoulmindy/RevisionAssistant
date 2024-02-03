cmake_minimum_required(VERSION 3.16)

project(RevisionAssistant VERSION 0.0.0 LANGUAGES CXX)

configure_file(config/config.h.in ${CMAKE_SOURCE_DIR}/config/config.h)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Core)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(RevisionAssistantApp
    src/main.cpp
)

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
)

target_link_libraries(RevisionAssistantApp
    PRIVATE Qt6::Quick Qt6::Core custom_componentsplugin
)

############################################# INSTALL
include(GNUInstallDirs)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX $ENV{HOME}/.local)
endif()

# Configure icon for Linux
if(UNIX AND NOT APPLE)
    set(DESKTOP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/config/org.Bsoulmindy.RevisionAssistant.desktop)
    set(ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/icons/revision_assistant_icon.png)

    set(DESKTOP_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/applications)
    set(ICON_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/icons)

    # Install the .desktop file
    install(FILES ${DESKTOP_FILE} DESTINATION ${DESKTOP_INSTALL_DIR})

    # Install the icon file
    install(FILES ${ICON_FILE} DESTINATION ${ICON_INSTALL_DIR} RENAME "revision-assistant.png")
endif()

install(TARGETS RevisionAssistantApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

list(APPEND QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/src")
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE PATH
    "Path to the custom QML components defined by the project" FORCE)
