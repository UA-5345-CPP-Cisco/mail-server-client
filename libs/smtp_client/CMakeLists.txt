add_library(mail_smtp_client
	src/MailBuilder.cpp
	src/SmtpClient.cpp
)

target_include_directories(mail_smtp_client
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(mail_smtp_client
    PUBLIC
	Boost::asio
	Boost::system
	Threads::Threads
)

target_compile_features(mail_smtp_client
	PUBLIC
	cxx_std_17
)
