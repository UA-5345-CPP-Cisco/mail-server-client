cmake_minimum_required(VERSION 3.16)

project(qtapptestmail VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(WINDOWS_DEV_CONSOLE OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql QuickControls2)
#find_package(SQLite3 REQUIRED)

if(NOT TARGET mail_storage)
	add_subdirectory(
			${CMAKE_CURRENT_SOURCE_DIR}/../../libs/mail-storage
			${CMAKE_CURRENT_BINARY_DIR}/libs/storage
	)
endif()

if(NOT TARGET mail_logger)
	add_subdirectory(
			${CMAKE_CURRENT_SOURCE_DIR}/../../libs/logger
			${CMAKE_CURRENT_BINARY_DIR}/libs/logger
	)
endif()

if(NOT TARGET Threads::Threads)
	find_package(Threads REQUIRED)
endif()

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
		models/database/RegistrationHandler.cpp
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
		headers/database/RegistrationHandler.h
		headers/users/AccountListModel.h
		models/users/AccountListModel.cpp
		models/database/RegistrationHandler.cpp
		models/color/ColorModel.cpp
		headers/color/ColorModel.h
		headers/color/ColorProvider.h
		models/color/ColorProvider.cpp
)

set_target_properties(appqtapptestmail PROPERTIES
		MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
		MACOSX_BUNDLE TRUE
)

if(WINDOWS_DEV_CONSOLE)
	set_target_properties(appqtapptestmail PROPERTIES
			WIN32_EXECUTABLE FALSE
	)
else()
	set_target_properties(appqtapptestmail PROPERTIES
			WIN32_EXECUTABLE TRUE
	)
endif()

target_link_libraries(appqtapptestmail
	PUBLIC
		mail_logger
		mail_storage
	PRIVATE
		Qt6::Quick
		Qt6::Sql
		Qt6::QuickControls2
)

target_include_directories(appqtapptestmail
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS appqtapptestmail
		BUNDLE DESTINATION .
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
