add_executable(mail_server_neo
    Main.cpp
    src/Common.cpp
    src/Server.cpp
    src/Session.cpp
    src/services/MailServerConfigurator.cpp
    src/services/MailServerLogger.cpp
    src/services/MailStorage.cpp
    src/services/ServiceRegistry.cpp
    src/handlers/GetMailsHandler.cpp
    src/handlers/HelloHandler.cpp
    src/handlers/SendMailHandler.cpp
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
        mail_storage
        smtp_server_sdk
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
