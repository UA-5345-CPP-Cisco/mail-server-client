add_library(mail_smtp_server
    src/smtp/AuthService.cpp
    src/smtp/BoostSocketsManager.cpp
    src/smtp/Configuration.cpp
    src/smtp/QueueDispatcher.cpp
    src/smtp/Server.cpp
    src/smtp/SmtpSessionHandler.cpp
    src/smtp/Session.cpp
)

target_include_directories(mail_smtp_server
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(mail_smtp_server
    PUBLIC
	Boost::asio
	Boost::system
	OpenSSL::SSL
	OpenSSL::Crypto
	Threads::Threads
	mail_logger
	mail_storage
	mail_thread_pool
	PRIVATE
	Boost::property_tree
	cryptopp::cryptopp
)

target_compile_features(mail_smtp_server
    PUBLIC
    cxx_std_17
)

target_compile_definitions(mail_smtp_server
	PUBLIC
		BOOST_ERROR_CODE_HEADER_ONLY
)

add_executable(smtp_server
    main.cpp
)

target_link_libraries(smtp_server 
    mail_smtp_server
)

if(WIN32)
    target_compile_definitions(mail_smtp_server PUBLIC _WIN32_WINNT=0x0A00)
    target_link_libraries(mail_smtp_server PUBLIC ws2_32 mswsock crypt32)
endif()

if(MSVC)
    target_compile_options(mail_smtp_server PRIVATE /W4) 
    target_compile_options(smtp_server PRIVATE /W4)

    if(MAIL_WARNINGS_AS_ERRORS)
        target_compile_options(mail_smtp_server PRIVATE /WX) 
        target_compile_options(smtp_server PRIVATE /WX)
    endif()
else()
    target_compile_options(
        mail_smtp_server 
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
        target_compile_options(mail_smtp_server PRIVATE -Werror) 
        target_compile_options(smtp_server PRIVATE -Werror)
    endif()
endif()