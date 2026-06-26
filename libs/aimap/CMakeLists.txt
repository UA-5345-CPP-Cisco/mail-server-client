add_library(mail_aimap
        src/Server.cpp
        src/Client.cpp
        include/aimap/Server.h
        include/aimap/Client.h
)

target_include_directories(mail_aimap
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(mail_aimap
        PUBLIC
        mail_logger
        mail_thread_pool
        Threads::Threads
        PRIVATE
        nlohmann_json::nlohmann_json
)

target_compile_features(mail_aimap
        PUBLIC
        cxx_std_17
)

if (MSVC)
    target_compile_options(mail_aimap PRIVATE /W4)

    if (MAIL_WARNINGS_AS_ERRORS)
        target_compile_options(mail_aimap PRIVATE /WX)
    endif ()
else ()
    target_compile_options(mail_aimap PRIVATE -Wall -Wextra -Wpedantic)

    if (MAIL_WARNINGS_AS_ERRORS)
        target_compile_options(mail_aimap PRIVATE -Werror)
    endif ()
endif ()