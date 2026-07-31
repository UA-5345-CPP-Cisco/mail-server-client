cmake_minimum_required(VERSION 3.16)

project(qtapptestmail VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(WINDOWS_DEV_CONSOLE ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql QuickControls2)
#find_package(SQLite3 REQUIRED)

qt_standard_project_setup(REQUIRES 6.10)

include(FetchContent)

# --- Crypto++ (cryptopp-modern) ---
if(NOT TARGET cryptopp)
	FetchContent_Declare(
			cryptopp_modern
			GIT_REPOSITORY https://github.com/cryptopp-modern/cryptopp-modern/
			GIT_TAG 2026.3.0
	)
	set(CRYPTOPP_BUILD_TESTING OFF CACHE BOOL "" FORCE)
	set(CRYPTOPP_INSTALL OFF CACHE BOOL "" FORCE)

	FetchContent_MakeAvailable(cryptopp_modern)
endif()

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

qt_add_qml_module(qtappmail_core
		URI qtapptestmail.core
		VERSION 1.0
		STATIC
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
		headers/database/AuthHandler.h
		headers/users/AccountListModel.h
		headers/service/Service.h
		headers/client_logger/ClientProxyLogger.h
		headers/client_logger/ClientConfigReader.h
		headers/color/ColorModel.h
		headers/color/ColorProvider.h

		models/database/AuthHandler.cpp
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
		models/users/AccountListModel.cpp
		models/service/Service.cpp
		models/client_logger/ClientProxyLogger.cpp
		models/client_logger/ClientConfigReader.cpp
		models/color/ColorModel.cpp
		models/color/ColorProvider.cpp
)

target_include_directories(qtappmail_core
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(qtappmail_core
		PUBLIC
		mail_logger
		mail_storage
		Qt6::Quick
		Qt6::Sql
		Qt6::Core
		Qt6::QuickControls2
		cryptopp
)

target_compile_definitions(qtappmail_core PUBLIC
		CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}/config"
)

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
		screens/email_list/StatePopup.qml
		RESOURCES
		resources.qrc
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
		qtappmail_core
		Qt6::Quick
		Qt6::Sql
		Qt6::QuickControls2
		cryptopp
)

target_include_directories(appqtapptestmail
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/headers
)

install(TARGETS appqtapptestmail
		BUNDLE DESTINATION .
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
