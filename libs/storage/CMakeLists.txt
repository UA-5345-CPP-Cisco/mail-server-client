find_package(SQLite3 REQUIRED)

add_library(mail_storage
    src/Database.cpp
    src/MailMessageRepository.cpp
    src/MigrationRunner.cpp
    src/MessageRecipientRepository.cpp
    src/Statement.cpp
    src/UserRepository.cpp
    include/storage/Database.h
    include/storage/MailMessageRecord.h
    include/storage/MailMessageRepository.h
    include/storage/MigrationRunner.h
    include/storage/MessageRecipientRecord.h
    include/storage/MessageRecipientRepository.h
    include/storage/Statement.h
    include/storage/UserRecord.h
    include/storage/UserRepository.h
)

target_include_directories(mail_storage
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(mail_storage
    PUBLIC
        SQLite::SQLite3
)

target_compile_features(mail_storage
    PUBLIC
        cxx_std_17
)