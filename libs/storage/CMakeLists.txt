add_library(mail_storage
		src/Database.cpp
		src/MailMessageRepository.cpp
		src/MigrationRunner.cpp
		src/MessageRecipientRepository.cpp
		src/Statement.cpp
		src/UserRepository.cpp
)
target_include_directories(mail_storage
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(mail_storage PUBLIC SQLite::SQLite3)

target_compile_features(mail_storage PUBLIC cxx_std_17)