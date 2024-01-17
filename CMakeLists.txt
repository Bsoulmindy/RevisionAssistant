cmake_minimum_required(VERSION 3.16)

project(RevisionAssistant VERSION 0.0.0 LANGUAGES CXX)

configure_file(config.h.in ${CMAKE_SOURCE_DIR}/config/config.h)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Sql Core)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appRevisionAssistant
    main.cpp
)

qt_add_qml_module(appRevisionAssistant
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
        src/views/FileSelectionStatus.qml
        src/views/DictProcessingPage.qml
    RESOURCES resources.qrc
    SOURCES dictcontroller.h dictcontroller.cpp
    SOURCES quizcontroller.h quizcontroller.cpp
    SOURCES filecontroller.h filecontroller.cpp
)

add_subdirectory(src/CustomComponents)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appRevisionAssistant PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appRevisionAssistant
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appRevisionAssistant
    PRIVATE Qt6::Quick Qt6::Sql Qt6::Core custom_componentsplugin
)

include(GNUInstallDirs)
install(TARGETS appRevisionAssistant
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

list(APPEND QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/src")
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE PATH
    "Path to the custom QML components defined by the project" FORCE)
