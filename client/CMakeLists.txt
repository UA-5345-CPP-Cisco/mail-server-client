cmake_minimum_required(VERSION 3.16)
set(CMAKE_PREFIX_PATH "D:/QT/6.11.1/mingw_64")

project(qtapptestmail VERSION 0.1 LANGUAGES CXX C)

#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

# Встановлюємо стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

		screens/navigation/account/AddAccountQML.qml
		screens/navigation/account/RegistrationQML.qml
		screens/navigation/account/LoginQML.qml
		screens/navigation/account/AccountItem.qml
		screens/email_list/EmailToolTip.qml
	RESOURCES
	        resources.qrc
	SOURCES
	        headers/database/Database.h
		headers/database/RegistrationHandler.h
		headers/database/MailMessageRecord.h
		headers/database/MailMessageRepository.h
		headers/database/MessageRecipientRecord.h
		headers/database/MessageRecipientRepository.h
		headers/database/MigrationRunner.h
		headers/database/Statement.h
		headers/database/UserRecord.h
		headers/database/UserRepository.h
		headers/database/CacheStore.h
		headers/database/DatabaseManager.h
		headers/mail/EmailCache.h
		headers/mail/EmailFilterProxy.h
		headers/mail/EmailListModel.h
		headers/mail/EmailPageProxy.h
		headers/mail/MessageComposer.h
		headers/search/MessageSearchModel.h
		headers/users/CurrentUser.h
		headers/users/UserCache.h
		models/database/Database.cpp
		models/database/MailMessageRepository.cpp
		models/database/MessageRecipientRepository.cpp
		models/database/MigrationRunner.cpp
		models/database/Statement.cpp
		models/database/RegistrationHandler.cpp
		models/database/UserRepository.cpp
		models/database/CacheStore.cpp
		models/database/DatabaseManager.cpp
		models/mail/EmailCache.cpp
		models/mail/EmailFilterProxy.cpp
		models/mail/EmailListModel.cpp
		models/mail/EmailPageProxy.cpp
		models/mail/MessageComposer.cpp
		models/search/MessageSearchModel.cpp
		models/users/CurrentUser.cpp
		models/users/UserCache.cpp
		headers/users/AccountListModel.h
		models/users/AccountListModel.cpp
		headers/database/RegistrationHandler.h
		models/database/RegistrationHandler.cpp

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
