cmake_minimum_required(VERSION 3.16)

project(qtapptestmail VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appqtapptestmail
    main.cpp
)
file(GLOB ASSET_FILES "assets/*")

foreach(asset_file ${ASSET_FILES})
    get_filename_component(file_name ${asset_file} NAME)
    set_source_files_properties(${asset_file} PROPERTIES QT_RESOURCE_ALIAS "assets/${file_name}")
endforeach()

qt_add_qml_module(appqtapptestmail
    URI qtapptestmail
    QML_FILES
        Main.qml
        NavigationQML.qml
        NewMessageQML.qml
        EmailsListQML.qml
        DraftsListQML.qml
        ContentBlankPageQML.qml
        ContentPageLetterQML.qml
        SettingsQML.qml
        SwitchAccountQML.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appqtapptestmail PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appqtapptestmail
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqtapptestmail
    PRIVATE Qt6::Quick
)

install(TARGETS appqtapptestmail
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
