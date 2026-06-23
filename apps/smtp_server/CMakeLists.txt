
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(cryptopp CONFIG) 

add_library(smtp_server_core
    src/smtp/AuthService.cpp
    src/smtp/BoostSocketsManager.cpp
    src/smtp/Configuration.cpp
    src/smtp/QueueDispatcher.cpp
    src/smtp/Server.cpp
    src/smtp/Session.cpp
    src/smtp/SmtpSessionHandler.cpp
    include/SMTP_Server.hpp
    include/smtp/AuthService.hpp
    include/smtp/Configuration.hpp
    include/smtp/Event.hpp
    include/smtp/QueueDispatcher.hpp
    include/smtp/Server.hpp
    include/smtp/ServerConfig.hpp
    include/smtp/Session.hpp
    include/smtp/SocketsManager.hpp
)

target_include_directories(smtp_server_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(smtp_server_core
    PUBLIC
        Boost::system
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
        mail_logger
        mail_storage
        mail_thread_pool
    PRIVATE
        cryptopp
)

target_compile_features(smtp_server_core
    PUBLIC
        cxx_std_17
)

target_compile_definitions(smtp_server_core
    PUBLIC
        BOOST_ERROR_CODE_HEADER_ONLY
)

if(WIN32)
    target_compile_definitions(smtp_server_core PUBLIC _WIN32_WINNT=0x0A00)
    target_link_libraries(smtp_server_core PUBLIC ws2_32 mswsock crypt32)
endif()

add_executable(smtp_server
    main.cpp
)
 
target_link_libraries(smtp_server
    PRIVATE
        smtp_server_core
)

target_compile_features(smtp_server
    PRIVATE
        cxx_std_17
)

if(MSVC)
    target_compile_options(smtp_server_core PRIVATE /W4)
    target_compile_options(smtp_server PRIVATE /W4)

    if(MAIL_WARNINGS_AS_ERRORS)
        target_compile_options(smtp_server_core PRIVATE /WX)
        target_compile_options(smtp_server PRIVATE /WX)
    endif()
else()
    target_compile_options(
        smtp_server_core
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
    target_compile_options(
        smtp_server
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )

    if(MAIL_WARNINGS_AS_ERRORS)
        target_compile_options(smtp_server_core PRIVATE -Werror)
        target_compile_options(smtp_server PRIVATE -Werror)
    endif()
endif()