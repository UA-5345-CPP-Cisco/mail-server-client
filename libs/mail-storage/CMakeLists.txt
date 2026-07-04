add_library(mail_storage
        src/Database.cpp
        src/MailMessageRepository.cpp
        src/MigrationRunner.cpp
        src/MessageRecipientRepository.cpp
        src/Statement.cpp
        src/UserRepository.cpp
        include/mail_storage/Database.h
        include/mail_storage/MailMessageRecord.h
        include/mail_storage/MailMessageRepository.h
        include/mail_storage/MigrationRunner.h
        include/mail_storage/MessageRecipientRecord.h
        include/mail_storage/MessageRecipientRepository.h
        include/mail_storage/Statement.h
        include/mail_storage/UserRecord.h
        include/mail_storage/UserRepository.h
)

if(NOT TARGET SQLite3::SQLite3 AND NOT TARGET SQLite::SQLite3)
    find_package(SQLite3 QUIET)
endif()

if(NOT TARGET SQLite3::SQLite3 AND NOT TARGET SQLite::SQLite3)
    include(FetchContent)

    FetchContent_Declare(
            sqlite3
            GIT_REPOSITORY https://github.com/sjinks/sqlite3-cmake.git
            GIT_TAG v3.49.1
    )
    FetchContent_MakeAvailable(sqlite3)
endif()

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
