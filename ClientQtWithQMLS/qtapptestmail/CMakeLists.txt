cmake_minimum_required(VERSION 3.16)

project(qtapptestmail VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
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
        screens/navigation/NavigationQML.qml
        screens/navigation/new_message/NewMessageQML.qml
        screens/email_list/EmailsListQML.qml
        screens/email_list/DraftsListQML.qml
        screens/content/ContentBlankPageQML.qml
        screens/content/ContentPageLetterQML.qml
        screens/navigation/settings/SettingsQML.qml
        screens/navigation/account/SwitchAccountQML.qml
        screens/email_list/ListItem.qml
        screens/email_list/DraftItem.qml
        screens/navigation/settings/SettingsGeneralQML.qml
        screens/navigation/account/AddAccountQML.qml
        RESOURCES
        resources.qrc
        SOURCES
        headers/mail/emaillistmodel.h
        models/mail/emaillistmodel.cpp
        headers/mail/emailfilterproxy.h
        models/mail/emailfilterproxy.cpp
        headers/mail/emailpageproxy.h
        models/mail/emailpageproxy.cpp
        headers/users/currentuser.h
        headers/database/databasemanager.h
        headers/hotkeys/hotkeysmanager.h
        headers/users/avatarimagegeneration.h
        models/users/currentuser.cpp
        models/search/messagesearchmodel.cpp
        headers/search/messagesearchmodel.h
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
