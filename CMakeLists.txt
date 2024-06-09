cmake_minimum_required(VERSION 3.16)

project(RevisionAssistant VERSION 0.3.1 LANGUAGES CXX)

# configuration file
configure_file(config/config.h.in ${CMAKE_SOURCE_DIR}/config/config.h)
configure_file(${CMAKE_SOURCE_DIR}/config/vercel.json ${CMAKE_BINARY_DIR}/vercel.json COPYONLY)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# recognize the app as executable if compiled with GCC Linux
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpie")
if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fwasm-exceptions")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fwasm-exceptions")
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Core Widgets QuickTest Test)

qt_standard_project_setup(REQUIRES 6.5)


qt_add_executable(RevisionAssistantApp_Tests
    tests/tst_main.cpp
    tests/testgroup/tst_group_interface.h
    tests/testgroup/tst_dict_repo_factory.cpp
    tests/tst_setup.h tests/tst_setup.cpp
    tests/testgroup/tst_dict_repo_factory.h
    tests/testgroup/tst_dict_json_repo.h tests/testgroup/tst_dict_json_repo.cpp
    tests/testgroup/tst_filesystemutils.h tests/testgroup/tst_filesystemutils.cpp
)
qt_add_executable(RevisionAssistantApp_QMLTests
    tests/tst_qml_main.cpp
    tests/tst_setup.h tests/tst_setup.cpp
)

qt_add_qml_module(RevisionAssistantApp_QMLTests
    URI RevisionAssistantAppTests
    VERSION 1.0
    QML_FILES tests/tst_dictcontroller.qml
    QML_FILES tests/tst_quizcontroller.qml
    tests/tst_dictfilescontroller.qml
    QML_FILES tests/tst_filecontroller.qml
    QML_FILES tests/tst_fileselectorcontroller.qml
)

set_target_properties(RevisionAssistantApp_QMLTests PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(RevisionAssistantApp_Tests PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Test
    Qt6::Widgets appplugin custom_componentsplugin
)
target_link_libraries(RevisionAssistantApp_QMLTests PRIVATE
    Qt6::QuickTest
    Qt6::Quick Qt6::Core Qt6::Widgets appplugin custom_componentsplugin
)

add_subdirectory(src/CustomComponents)
add_subdirectory(src)

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
    PRIVATE Qt6::Quick Qt6::Core Qt6::Widgets appplugin custom_componentsplugin
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

############################################# TEST
enable_testing(true)
add_test(NAME RevisionAssistantApp_Tests COMMAND RevisionAssistantApp_Tests)
add_test(NAME RevisionAssistantApp_QMLTests COMMAND RevisionAssistantApp_QMLTests)
