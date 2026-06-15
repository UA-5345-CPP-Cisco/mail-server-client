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

add_executable(smtp_client_tests
	smtp_client/SmtpClientTests.cpp
)

target_link_libraries(smtp_client_tests
	PRIVATE
		smtp_client
		GTest::gtest_main
)

target_compile_features(smtp_client_tests
	PRIVATE
		cxx_std_17
)

gtest_discover_tests(smtp_client_tests)

add_executable(logger_tests
	logger/LoggerTests.cpp
)

target_link_libraries(logger_tests
	PRIVATE
		mail_logger
		GTest::gtest_main
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
		GTest::gtest_main
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
		GTest::gtest_main
)

target_compile_features(smtp_server_configuration_tests
	PRIVATE
		cxx_std_17
)

target_compile_definitions(smtp_server_configuration_tests
	PRIVATE
		SMTP_SERVER_TEST_MIGRATIONS_DIR="${PROJECT_SOURCE_DIR}/libs/storage/migrations"
)

gtest_discover_tests(smtp_server_configuration_tests)
