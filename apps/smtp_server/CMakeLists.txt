add_library(smtp_server_core
	src/smtp/BoostSocketsManager.cpp
	src/smtp/Logger.cpp
	src/smtp/Server.cpp
	src/smtp/Services.cpp
	src/smtp/Session.cpp
	src/smtp/SmtpSessionHandler.cpp
	src/smtp/ThreadPool.cpp
	include/SMTP_Server.hpp
	include/smtp/Event.hpp
	include/smtp/Logger.hpp
	include/smtp/Server.hpp
	include/smtp/ServerConfig.hpp
	include/smtp/Services.hpp
	include/smtp/Session.hpp
	include/smtp/SocketsManager.hpp
	include/smtp/ThreadPool.hpp
)

target_include_directories(smtp_server_core
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(smtp_server_core
	PUBLIC
		Boost::asio
		Boost::system
		OpenSSL::SSL
		OpenSSL::Crypto
		Threads::Threads
	PRIVATE
		cryptopp::cryptopp
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
		mail_storage
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
