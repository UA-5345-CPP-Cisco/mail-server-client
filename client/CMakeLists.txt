cmake_minimum_required(VERSION 3.16)

project(qtapptestmail VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql QuickControls2)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appqtapptestmail
	main.cpp
	include/sqlite3/sqlite3.c
)

target_include_directories(appqtapptestmail PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/sqlite3")

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
		screens/content/ContentBlankPageQML.qml
		screens/content/ContentPageLetterQML.qml
		screens/navigation/settings/SettingsQML.qml
		screens/navigation/account/SwitchAccountQML.qml
		screens/email_list/ListItem.qml
		screens/email_list/DraftItem.qml
		screens/navigation/settings/SettingsGeneralQML.qml
		screens/navigation/account/AddAccountQML.qml
		screens/navigation/account/RegistrationQML.qml
		screens/navigation/account/LoginQML.qml
		screens/navigation/account/AccountItem.qml
		screens/email_list/EmailToolTip.qml
	RESOURCES
	        resources.qrc
	SOURCES
	        headers/database/Database.h
		headers/database/MailMessageRecord.h
		headers/database/MailMessageRepository.h
		headers/database/MessageRecipientRecord.h
		headers/database/MessageRecipientRepository.h
		headers/database/MigrationRunner.h
		headers/database/Statement.h
		headers/database/UserRecord.h
		headers/database/UserRepository.h
		headers/database/cachestore.h
		headers/database/databasemanager.h
		headers/hotkeys/hotkeysmanager.h
		headers/mail/emailcache.h
		headers/mail/emailfilterproxy.h
		headers/mail/emaillistmodel.h
		headers/mail/emailpageproxy.h
		headers/mail/messagecomposer.h
		headers/search/messagesearchmodel.h
		headers/users/avatarimagegeneration.h
		headers/users/currentuser.h
		headers/users/usercache.h
		models/database/Database.cpp
		models/database/MailMessageRepository.cpp
		models/database/MessageRecipientRepository.cpp
		models/database/MigrationRunner.cpp
		models/database/Statement.cpp
		models/database/UserRepository.cpp
		models/database/cachestore.cpp
		models/database/databasemanager.cpp
		models/mail/emailcache.cpp
		models/mail/emailfilterproxy.cpp
		models/mail/emaillistmodel.cpp
		models/mail/emailpageproxy.cpp
		models/mail/messagecomposer.cpp
		models/search/messagesearchmodel.cpp
		models/users/currentuser.cpp
		models/users/usercache.cpp
		headers/users/accountlistmodel.h
		models/users/accountlistmodel.cpp
)

set_target_properties(appqtapptestmail PROPERTIES
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqtapptestmail
	PRIVATE
	        Qt6::Quick
		Qt6::Sql
		Qt6::QuickControls2
)

install(TARGETS appqtapptestmail
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
