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
