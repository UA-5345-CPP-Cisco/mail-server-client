add_library(mail_storage
    src/Database.cpp
    src/MessageRecipientRepository.cpp
    src/MigrationRunner.cpp
    src/Statement.cpp
    src/UserRepository.cpp
    src/MailMessageRepository.cpp
)

target_include_directories(mail_storage
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(mail_storage
    PRIVATE
    sqlite3
)

target_compile_features(mail_storage
    PUBLIC
	cxx_std_17
)