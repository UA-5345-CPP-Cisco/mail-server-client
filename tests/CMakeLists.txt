add_executable(mail_storage_tests
        mail_storage/DatabaseTests.cpp
        mail_storage/MailDeliveryRepositoryTests.cpp
        mail_storage/MigrationRunnerTests.cpp
        mail_storage/StatementTests.cpp
        mail_storage/UserRepositoryTests.cpp
)

target_link_libraries(mail_storage_tests
        PRIVATE
        mail_storage
        gtest_main
        gmock
)

target_compile_features(mail_storage_tests
        PRIVATE
        cxx_std_17
)

target_compile_definitions(mail_storage_tests
        PRIVATE
        STORAGE_TEST_MIGRATIONS_DIR="${PROJECT_SOURCE_DIR}/libs/mail-storage/migrations"
)

include(GoogleTest)
gtest_discover_tests(mail_storage_tests)

add_executable(smtp_server_sdk_tests
        smtp_server_sdk/MailBuilderTests.cpp
)

target_link_libraries(smtp_server_sdk_tests
        PRIVATE
        smtp_server_sdk
        gtest_main
)

target_compile_features(smtp_server_sdk_tests
        PRIVATE
        cxx_std_17
)

gtest_discover_tests(smtp_server_sdk_tests)

add_executable(logger_tests
        logger/LoggerTests.cpp
)

target_link_libraries(logger_tests
        PRIVATE
        mail_logger
        gtest_main
)

target_compile_features(logger_tests
        PRIVATE
        cxx_std_17
)

gtest_discover_tests(logger_tests)

add_executable(thread_pool_tests
        thread_pool/ThreadPoolTests.cpp
)

target_link_libraries(thread_pool_tests
        PRIVATE
        mail_thread_pool
        gtest_main
)

target_compile_features(thread_pool_tests
        PRIVATE
        cxx_std_17
)

gtest_discover_tests(thread_pool_tests)

add_executable(smtp_server_configuration_tests
        smtp_server/ConfigurationTests.cpp
        smtp_server/QueueDispatcherTests.cpp
        smtp_server/SessionStorageTests.cpp
)

target_link_libraries(smtp_server_configuration_tests
        PRIVATE
        smtp_server_core
        gtest_main
)

target_compile_features(smtp_server_configuration_tests
        PRIVATE
        cxx_std_17
)

target_compile_definitions(smtp_server_configuration_tests
        PRIVATE
        SMTP_SERVER_TEST_MIGRATIONS_DIR="${PROJECT_SOURCE_DIR}/libs/mail-storage/migrations"
)

gtest_discover_tests(smtp_server_configuration_tests)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Gui Sql REQUIRED)

add_executable(state_tests
        state_tests/CurrentUserTests.cpp
		state_tests/AccountListModelTests.cpp
		state_tests/EmailCacheTests.cpp
		state_tests/UserCacheTests.cpp
		state_tests/EmailFilterProxyTests.cpp
		state_tests/EmailListModelTests.cpp
		../apps/qt-mail-client/models/users/CurrentUser.cpp
		../apps/qt-mail-client/models/users/UserCache.cpp
		../apps/qt-mail-client/models/mail/EmailCache.cpp
		../apps/qt-mail-client/models/mail/EmailListModel.cpp
		../apps/qt-mail-client/models/mail/EmailFilterProxy.cpp
		../apps/qt-mail-client/models/mail/EmailPageProxy.cpp
		../apps/qt-mail-client/models/users/AccountListModel.cpp
		../apps/qt-mail-client/models/database/CacheStore.cpp
		../apps/qt-mail-client/models/database/DatabaseManager.cpp
		../apps/qt-mail-client/headers/users/CurrentUser.h
		../apps/qt-mail-client/headers/users/UserCache.h
		../apps/qt-mail-client/headers/mail/EmailCache.h
		../apps/qt-mail-client/headers/mail/EmailListModel.h
		../apps/qt-mail-client/headers/mail/EmailFilterProxy.h
		../apps/qt-mail-client/headers/mail/EmailPageProxy.h
		../apps/qt-mail-client/headers/users/AccountListModel.h
		../apps/qt-mail-client/headers/database/CacheStore.h
		../apps/qt-mail-client/headers/database/DatabaseManager.h
)

target_include_directories(state_tests
		PRIVATE
		../apps/qt-mail-client
		../apps/qt-mail-client/headers
)

target_link_libraries(state_tests
		PRIVATE
		GTest::gtest
		gtest_main
		Qt6::Core
		Qt6::Sql
		Qt6::Gui
		mail_storage
)

target_compile_features(state_tests
		PRIVATE
		cxx_std_17
)

gtest_discover_tests(state_tests DISCOVERY_MODE PRE_TEST)