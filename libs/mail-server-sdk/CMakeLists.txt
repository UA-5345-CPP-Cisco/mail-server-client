add_library(mail_server_sdk
        src/Server.cpp
        src/Client.cpp
        include/mail_server_sdk/Server.h
        include/mail_server_sdk/Client.h
)

target_include_directories(mail_server_sdk
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(mail_server_sdk
        PUBLIC
        mail_logger
        mail_thread_pool
        Threads::Threads
        #PRIVATE
        nlohmann_json::nlohmann_json
)

target_compile_features(mail_server_sdk
        PUBLIC
        cxx_std_17
)

if (MSVC)
    target_compile_options(mail_server_sdk PRIVATE /W4)

    if (MAIL_WARNINGS_AS_ERRORS)
        target_compile_options(mail_server_sdk PRIVATE /WX)
    endif ()
else ()
    target_compile_options(mail_server_sdk PRIVATE -Wall -Wextra -Wpedantic)

    if (MAIL_WARNINGS_AS_ERRORS)
        target_compile_options(mail_server_sdk PRIVATE -Werror)
    endif ()
endif ()