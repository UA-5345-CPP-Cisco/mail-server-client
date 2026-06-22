add_library(smtp_client
	src/MailBuilder.cpp
	src/SmtpClient.cpp
	include/smtp_client/MailBuilder.h
	include/smtp_client/SmtpClient.h
)

target_include_directories(smtp_client
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(smtp_client
	PUBLIC
		Boost::asio
		Boost::system
		Threads::Threads
)

target_compile_features(smtp_client
	PUBLIC
		cxx_std_17
)
