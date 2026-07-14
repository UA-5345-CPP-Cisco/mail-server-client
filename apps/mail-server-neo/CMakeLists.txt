add_executable(mail_server_neo
    main.cpp
    src/common.cpp
    src/server.cpp
    src/session.cpp
    src/handlers/hello_handler.cpp
)

target_include_directories(mail_server_neo
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(mail_server_neo
        PRIVATE
        Boost::asio
        Boost::beast
        Boost::json
        Boost::system

        mail_logger
        mail_thread_pool

        Threads::Threads
)

target_compile_features(mail_server_neo
        PRIVATE
        cxx_std_17
)

if(MSVC)
    target_compile_options(mail_server_neo PRIVATE /W4)
else()
    target_compile_options(mail_server_neo PRIVATE -Wall -Wextra -Wpedantic)
endif()