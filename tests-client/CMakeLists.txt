include(GoogleTest)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Gui Sql REQUIRED)

add_executable(state_tests
		../tests-client/state_tests/CurrentUserTests.cpp
		../tests-client/state_tests/AccountListModelTests.cpp
		../tests-client/state_tests/EmailCacheTests.cpp
		../tests-client/state_tests/UserCacheTests.cpp
		../tests-client/state_tests/EmailFilterProxyTests.cpp
		../tests-client/state_tests/EmailListModelTests.cpp
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
		state_tests/main.cpp
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

gtest_discover_tests(state_tests)