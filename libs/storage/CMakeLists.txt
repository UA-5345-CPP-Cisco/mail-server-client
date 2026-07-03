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

include(FetchContent)

FetchContent_Declare(
        sqlite3
        GIT_REPOSITORY https://github.com/sjinks/sqlite3-cmake.git
        GIT_TAG v3.49.1
)
FetchContent_MakeAvailable(sqlite3)

target_include_directories(mail_storage
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(TARGET SQLite3::SQLite3)
    target_link_libraries(mail_storage
        PUBLIC
            SQLite3::SQLite3
    )
elseif(TARGET SQLite::SQLite3)
    target_link_libraries(mail_storage
        PUBLIC
            SQLite::SQLite3
    )
else()
    message(FATAL_ERROR "SQLite target was not found")
endif()

target_compile_features(mail_storage
    PUBLIC
        cxx_std_17
)
