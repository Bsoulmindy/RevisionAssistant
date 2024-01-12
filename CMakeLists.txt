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

file(GLOB_RECURSE QMLSOURCES_ABSOLUTE "*.qml")
set(QMLSOURCES "")
foreach(QMLFILE_ABSOLUTE ${QMLSOURCES_ABSOLUTE})
    file(RELATIVE_PATH QMLFILE_RELATIVE ${CMAKE_SOURCE_DIR} ${QMLFILE_ABSOLUTE})
    list(APPEND QMLSOURCES ${QMLFILE_RELATIVE})
endforeach()

qt_add_qml_module(appRevisionAssistant
    URI RevisionAssistant
    VERSION 1.0
    QML_FILES Main.qml CompleteButton.qml DictEditPage.qml src/views/MainPage.qml src/views/MainPageUi.ui.qml QrPage.qml DictAddPage.qml FileSelectionStatus.qml DictProcessingPage.qml
    RESOURCES resources.qrc
    SOURCES dictcontroller.h dictcontroller.cpp
    SOURCES quizcontroller.h quizcontroller.cpp
    SOURCES filecontroller.h filecontroller.cpp
    QML_FILES src/components/PrimaryText.qml
)

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
    PRIVATE Qt6::Quick Qt6::Sql Qt6::Core
)

include(GNUInstallDirs)
install(TARGETS appRevisionAssistant
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(QML_IMPORT_PATH ${PROJECT_BINARY_DIR}/qml CACHE PATH
    "Path to the custom QML components defined by the project")
