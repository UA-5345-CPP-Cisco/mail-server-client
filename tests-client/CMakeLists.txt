find_package(Qt6 COMPONENTS Core Gui Sql REQUIRED)

add_library(state_tests_lib
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/users/CurrentUser.cpp
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/users/UserCache.cpp
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/mail/EmailCache.cpp
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/mail/EmailListModel.cpp
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/mail/EmailFilterProxy.cpp
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/mail/EmailPageProxy.cpp
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/users/AccountListModel.cpp
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/database/CacheStore.cpp
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/database/DatabaseManager.cpp
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/mail/MessageComposer.cpp
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/database/RegistrationHandler.cpp
        ${PROJECT_SOURCE_DIR}/apps/mail-client/models/search/MessageSearchModel.cpp

        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/search/MessageSearchModel.h
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/database/RegistrationHandler.h
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/users/CurrentUser.h
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/users/UserCache.h
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/mail/EmailCache.h
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/mail/EmailListModel.h
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/mail/EmailFilterProxy.h
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/mail/EmailPageProxy.h
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/users/AccountListModel.h
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/database/CacheStore.h
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/database/DatabaseManager.h
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers/mail/MessageComposer.h
)

set_target_properties(state_tests_lib PROPERTIES AUTOMOC ON)

target_include_directories(state_tests_lib
        PUBLIC
        ${PROJECT_SOURCE_DIR}/apps/mail-client/headers
        ${PROJECT_SOURCE_DIR}/apps/mail-client

)

target_link_libraries(state_tests_lib
        PUBLIC
        Qt6::Core
        Qt6::Sql
        Qt6::Gui
        mail_storage
)

add_executable(state_tests
        state_tests/CurrentUserTests.cpp
        state_tests/AccountListModelTests.cpp
        state_tests/EmailCacheTests.cpp
        state_tests/UserCacheTests.cpp
        state_tests/EmailFilterProxyTests.cpp
        state_tests/EmailListModelTests.cpp
        model_tests/MessageComposerTests.cpp
        model_tests/RegistrationHandlerTests.cpp
        model_tests/MessageSearchModelTests.cpp
        model_tests/DatabaseManagerTests.cpp
)

target_link_libraries(state_tests
        PRIVATE
        state_tests_lib
        GTest::gtest
        gtest_main

)

target_compile_features(state_tests
        PRIVATE
        cxx_std_17
)

target_compile_definitions(state_tests
        PRIVATE
        STATE_TEST_MIGRATIONS_DIR="${PROJECT_SOURCE_DIR}/libs/mail-storage/migrations"
)

gtest_discover_tests(state_tests DISCOVERY_MODE PRE_TEST)