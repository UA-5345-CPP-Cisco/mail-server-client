cmake_policy(SET CMP0135 NEW)
add_compile_definitions(STORAGE_TEST_MIGRATIONS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../libs/storage/migrations")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)


add_executable(run_logger_test logger/logger_test.cpp)
target_link_libraries(run_logger_test PRIVATE mail_logger GTest::gtest_main GTest::gmock)

add_executable(run_thread_pool_test thread_pool/thread_pool_test.cpp)
target_link_libraries(run_thread_pool_test PRIVATE mail_thread_pool GTest::gtest_main GTest::gmock)


add_executable(run_imap_server_test imap_server/imap_server_test.cpp)
target_link_libraries(run_imap_server_test PRIVATE mail_backend_lib GTest::gtest_main GTest::gmock)

add_executable(run_smtp_server_tests 
    smtp_server/ConfigurationTests.cpp
    smtp_server/QueueDispatcherTests.cpp
    smtp_server/SessionStorageTests.cpp
)
target_link_libraries(run_smtp_server_tests PRIVATE smtp_server_core GTest::gtest_main GTest::gmock)

add_executable(run_smtp_client_tests smtp_client/SmtpClientTests.cpp)
target_link_libraries(run_smtp_client_tests PRIVATE smtp_client GTest::gtest_main GTest::gmock)


add_executable(run_storage_test 
    storage/DatabaseTests.cpp
    storage/MailDeliveryRepositoryTests.cpp
    storage/MigrationRunnerTests.cpp
    storage/StatementTests.cpp
    storage/UserRepositoryTests.cpp
)
target_link_libraries(run_storage_test PRIVATE mail_storage GTest::gtest_main GTest::gmock)

target_compile_definitions(run_storage_test PUBLIC 
    STORAGE_TEST_MIGRATIONS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../libs/storage/migrations"
)

include(GoogleTest)
gtest_discover_tests(run_logger_test)
gtest_discover_tests(run_thread_pool_test)
gtest_discover_tests(run_imap_server_test)
gtest_discover_tests(run_smtp_server_tests)
gtest_discover_tests(run_smtp_client_tests)
gtest_discover_tests(run_storage_test)