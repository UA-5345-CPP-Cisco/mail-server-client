add_executable(mail_imap_tests imap_server/imap_server_test.cpp)
target_link_libraries(mail_imap_tests
    PRIVATE
    imap_back_lib
    GTest::gtest_main
	GTest::gmock
)
target_compile_features(mail_imap_tests
	PRIVATE
		cxx_std_17
)
include(GoogleTest)
gtest_discover_tests(mail_imap_tests)


add_executable(mail_logger_tests logger/logger_test.cpp)
target_link_libraries(mail_logger_tests
    PRIVATE
    mail_logger
    GTest::gtest_main
	GTest::gmock
)
target_compile_features(mail_logger_tests
	PRIVATE
		cxx_std_17
)
include(GoogleTest)
gtest_discover_tests(mail_logger_tests)


add_executable(mail_smtp_client_tests
    smtp_client/SmtpClientTests.cpp
)
target_link_libraries(mail_smtp_client_tests
    PRIVATE
    mail_smtp_client
    GTest::gtest_main
	GTest::gmock
)
target_compile_features(mail_smtp_client_tests
	PRIVATE
		cxx_std_17
)
include(GoogleTest)
gtest_discover_tests(mail_smtp_client_tests)


#add_executable(mail_smtp_server_tests
#   smtp_server/ConfigurationTests.cpp
#    smtp_server/QueueDispatcherTests.cpp
#    smtp_server/SessionStorageTests.cpp
#)
#target_link_libraries(mail_smtp_server_tests
#    PRIVATE
#   mail_smtp_server
#    GTest::gtest_main
#	GTest::gmock
#)
#target_compile_features(mail_smtp_server_tests
#	PRIVATE
#		cxx_std_17
#)
#include(GoogleTest)
#gtest_discover_tests(mail_smtp_server_tests)


add_executable(mail_storage_tests
	storage/DatabaseTests.cpp
	storage/MailDeliveryRepositoryTests.cpp
	storage/MigrationRunnerTests.cpp
	storage/StatementTests.cpp
	storage/UserRepositoryTests.cpp
)

target_link_libraries(mail_storage_tests
	PRIVATE
		mail_storage
		GTest::gtest_main
		GTest::gmock
)

target_compile_features(mail_storage_tests
	PRIVATE
		cxx_std_17
)

target_compile_definitions(mail_storage_tests
	PRIVATE
		STORAGE_TEST_MIGRATIONS_DIR="${PROJECT_SOURCE_DIR}/libs/storage/migrations"
)

include(GoogleTest)
gtest_discover_tests(mail_storage_tests)


add_executable(mail_thread_pool_tests thread_pool/thread_pool_test.cpp)
target_link_libraries(mail_thread_pool_tests 
    PRIVATE
    mail_thread_pool
    GTest::gtest_main
	GTest::gmock
)
target_compile_features(mail_thread_pool_tests
	PRIVATE
		cxx_std_17
)
include(GoogleTest)
gtest_discover_tests(mail_thread_pool_tests)




