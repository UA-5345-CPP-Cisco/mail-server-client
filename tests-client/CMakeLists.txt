include(GoogleTest)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Gui Sql REQUIRED)

add_executable(state_tests
		state_tests/CurrentUserTests.cpp
		state_tests/AccountListModelTests.cpp
		state_tests/EmailCacheTests.cpp
		state_tests/UserCacheTests.cpp
		state_tests/EmailFilterProxyTests.cpp
		state_tests/EmailListModelTests.cpp
		state_tests/main.cpp
		../apps/mail-client/models/users/CurrentUser.cpp
		../apps/mail-client/models/users/UserCache.cpp
		../apps/mail-client/models/mail/EmailCache.cpp
		../apps/mail-client/models/mail/EmailListModel.cpp
		../apps/mail-client/models/mail/EmailFilterProxy.cpp
		../apps/mail-client/models/mail/EmailPageProxy.cpp
		../apps/mail-client/models/users/AccountListModel.cpp
		../apps/mail-client/models/database/CacheStore.cpp
		../apps/mail-client/models/database/DatabaseManager.cpp
		../apps/mail-client/headers/users/CurrentUser.h
		../apps/mail-client/headers/users/UserCache.h
		../apps/mail-client/headers/mail/EmailCache.h
		../apps/mail-client/headers/mail/EmailListModel.h
		../apps/mail-client/headers/mail/EmailFilterProxy.h
		../apps/mail-client/headers/mail/EmailPageProxy.h
		../apps/mail-client/headers/users/AccountListModel.h
		../apps/mail-client/headers/database/CacheStore.h
		../apps/mail-client/headers/database/DatabaseManager.h
)

target_include_directories(state_tests
		PRIVATE
		../apps/mail-client
		../apps/mail-client/headers
)

target_link_libraries(state_tests
		PRIVATE
		gtest
		Qt6::Core
		Qt6::Sql
		Qt6::Gui
		mail_storage
)

target_compile_features(state_tests
		PRIVATE
		cxx_std_17
)

#gtest_discover_tests(state_tests)

gtest_add_tests(TARGET state_tests SOURCES
		state_tests/CurrentUserTests.cpp
		state_tests/AccountListModelTests.cpp
		state_tests/EmailCacheTests.cpp
		state_tests/UserCacheTests.cpp
		state_tests/EmailFilterProxyTests.cpp
		state_tests/EmailListModelTests.cpp
)

add_executable(model_tests
		model_tests/DatabaseManagerTests.cpp
		model_tests/MessageComposerTests.cpp
		model_tests/MessageSearchModelTests.cpp
		model_tests/RegistrationHandlerTests.cpp

		state_tests/main.cpp

		../apps/mail-client/models/mail/MessageComposer.cpp
		../apps/mail-client/models/search/MessageSearchModel.cpp
		../apps/mail-client/models/database/RegistrationHandler.cpp
		../apps/mail-client/models/database/DatabaseManager.cpp

		../apps/mail-client/headers/mail/MessageComposer.h
		../apps/mail-client/headers/search/MessageSearchModel.h
		../apps/mail-client/headers/database/RegistrationHandler.h
		../apps/mail-client/headers/database/DatabaseManager.h
)

target_include_directories(model_tests
		PRIVATE
		../apps/mail-client
		../apps/mail-client/headers
)

target_link_libraries(model_tests
		PRIVATE
		gtest
		Qt6::Core
		Qt6::Sql
		Qt6::Gui
		mail_storage
)

target_compile_features(model_tests
		PRIVATE
		cxx_std_17
)

#gtest_discover_tests(model_tests)

gtest_add_tests(TARGET model_tests SOURCES
		model_tests/DatabaseManagerTests.cpp
		model_tests/MessageComposerTests.cpp
		model_tests/MessageSearchModelTests.cpp
		model_tests/RegistrationHandlerTests.cpp
)